#include <mesh.h>
#include <shaderloader.h>
#include <map>
#include <string>

ShaderProgram activeShader;
ShaderProgram activeShadowShader;



Mesh::MeshEntry::~MeshEntry()
{

}



Mesh::Mesh()
{
}


Mesh::~Mesh()
{
	Clear();
}

void Mesh::Clear()
{

}



bool Mesh::LoadMesh(const std::string& Filename) {
	//Clears previous loaded mesh
	Clear();


	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(sizeof(m_Buffers) / sizeof(m_Buffers[0]), m_Buffers);



	bool Ret = false;
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	if (pScene) {
		Ret = InitFromScene(pScene, Filename);
	}
	else {
		GFX_WARN("Error parsing %s: %s \n", Filename.c_str(), importer.GetErrorString());
	}

	glBindVertexArray(0);

	return Ret;
}

bool Mesh::InitFromScene(const aiScene* pScene, const std::string& Filename) {
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);

	std::vector<glm::vec3> Positions;
	std::vector<glm::vec3> Normals;
	std::vector<glm::vec2> TexCoords;
	std::vector<unsigned int> Indices;

	unsigned int numVertices = 0;
	unsigned int numIndices = 0;


	//Initialize the meshes one by one
	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
		m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
		m_Entries[i].BaseVertex = numVertices;
		m_Entries[i].BaseIndex = numIndices;

		numVertices += pScene->mMeshes[i]->mNumVertices;
		numIndices += m_Entries[i].NumIndices;


	}

	Positions.reserve(numVertices);
	Normals.reserve(numVertices);
	TexCoords.reserve(numVertices);
	Indices.reserve(numIndices);


	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(paiMesh, Positions, Normals, TexCoords, Indices);
	}

	if (!InitMaterials(pScene, Filename)) {
		return false;
	}


	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0],
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0],
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0],
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0],
		GL_STATIC_DRAW);
	//Create and bind uniform block
	unsigned int uniformBlock = glGetUniformBlockIndex(activeShader.getID(), "Matrices");
	glUniformBlockBinding(activeShader.getID(), uniformBlock, 0);

	glGenBuffers(1, &uboMatrices);

	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (GLfloat)1024 / (GLfloat)764, 0.1f, -10.0f);

	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(proj));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}

void Mesh::InitMesh(const aiMesh* paiMesh, std::vector<glm::vec3> &Positions, std::vector<glm::vec3> &Normals, std::vector<glm::vec2> &TexCoords, std::vector<unsigned int> &Indices) {

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		//GFX_DEBUG("X: %f, Y: %f, Z: %f", pPos->x, pPos->y, pPos->z);

		Positions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
		Normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
		TexCoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));
	}

	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& face = paiMesh->mFaces[i];
		assert(face.mNumIndices == 3);
		Indices.push_back(face.mIndices[0]);
		Indices.push_back(face.mIndices[1]);
		Indices.push_back(face.mIndices[2]);
	}
}


bool Mesh::InitMaterials(const aiScene* pScene, const std::string& Filename) {
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
		const aiMaterial* pMaterial = pScene->mMaterials[i];
		m_Textures[i] = NULL;
		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {


				std::string  filePath = Filename.substr(0, Filename.find_last_of("\\/"));

				std::string localPath = Path.C_Str();

				std::string fullPath = filePath + "/" + localPath;

				GFX_DEBUG("%s", Path.C_Str());
				m_Textures[i] = new Texture(GL_TEXTURE_2D, fullPath.c_str());

			}
		}
	}
	depthMap = new Texture();
	return true;
}


void Mesh::SetShader(char shader[]) {

	GFX_DEBUG(shader);

	std::string vert;
	std::string frag;
	std::string svert;
	std::string sfrag;
	vert.append("resources/shaders/").append(shader).append("/vertex.vert");
	frag.append("resources/shaders/").append(shader).append("/fragment.frag");
	//svert.append("resources/shaders/").append(shader).append("/shadow.vert");
	//sfrag.append("resources/shaders/").append(shader).append("/shadow.frag");
	activeShader = createProgram(vert, frag);
	//activeShadowShader = createProgram(svert, sfrag);

}

void Mesh::SetShadowShader(char vertexShader[], char fragmentShader[]) {
	activeShadowShader = createProgram(vertexShader, fragmentShader);
}

glm::mat4 Mesh::ShadowRenderer() {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	// 1. render depth of scene to texture (from light's perspective)
	// --------------------------------------------------------------
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 7.5f;
	for (unsigned int i = 0; i < m_Entries.size(); i++) {
	//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspectiv projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::vec3 lightPosition = glm::vec3(100, 100, 100);
	lightView = glm::lookAt(lightPosition, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	// render scene from light's point of view
	glViewport(0, 0, 1024, 1024);
	//glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap->ID);
	GLint lsm = activeShader.getUniformLocation("lightSpaceMatrix");

	glUniformMatrix4fv(lsm, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		GLint modelID;
		modelID = activeShader.getUniformLocation("model");

		glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
		glDrawElementsBaseVertex(GL_TRIANGLES,
			m_Entries[i].NumIndices, GL_UNSIGNED_INT,
			(void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex),
			m_Entries[i].BaseVertex);
	}
	// reset viewport
	glViewport(0, 0, 1024, 768);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return lightSpaceMatrix;
}



void Mesh::Render(CameraMovement* cam, glm::mat4 model, DirectionalLight* dirLight, std::vector<PointLight*> pointLights) {

	m_Lights = pointLights;
	m_dirLight = dirLight;

	static float time;
	glBindVertexArray(m_VAO);
	time += 0.00001f;
	//activeShadowShader = shaders.find("shadow")->second;
	//activeShadowShader.bind();
	//glm::mat4 lsm = this->ShadowRenderer();
	activeShader.bind();




	for (unsigned int i = 0; i < m_Entries.size(); i++) {

		const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

		assert(MaterialIndex < m_Textures.size());

		if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
			m_Textures[MaterialIndex]->Bind();
		}

		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, depthMap->ID);
		GLint viewID, projectionID, modelID;

		modelID = activeShader.getUniformLocation("model");

		//Viewport matrices
		glm::mat4 view = glm::lookAt(cam->cameraPos, cam->cameraPos + cam->cameraFront, cam->cameraUp);
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);


		//Sends model matrice to shader for transformation
		glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(model));


		GLint lightPosID, viewPosID, ambientID, diffuseID, specularID, shininessID, lightAID, lightDID, lightSID, lightSpaceMatrixID,
			pLightC, pLightL, pLightQ, lightNum, timeID;

		viewPosID = activeShader.getUniformLocation("aviewPos");
		glm::vec3 viewPosition = glm::vec3(cam->cameraPos.x, cam->cameraPos.y, cam->cameraPos.z);
		glUniform3fv(viewPosID, 1, glm::value_ptr(viewPosition));

		//timeID = activeShader.getUniformLocation("time");
		//glUniform1f(timeID, time);
		lightNum = activeShader.getUniformLocation("gNumOfLights");
		glUniform1i(lightNum, m_Lights.size());

		for (int i = 0; i < m_Lights.size(); i++) {


			std::string pointLight = "pointLights[";
			std::string number = std::to_string(i);

			lightPosID = activeShader.getUniformLocation((pointLight + number + "].position").c_str());
			lightAID = activeShader.getUniformLocation((pointLight + number + "].ambient").c_str());
			lightDID = activeShader.getUniformLocation((pointLight + number + "].diffuse").c_str());
			lightSID = activeShader.getUniformLocation((pointLight + number + "].specular").c_str());
			pLightC = activeShader.getUniformLocation((pointLight + number + "].constant").c_str());
			pLightL = activeShader.getUniformLocation((pointLight + number + "].linear").c_str());
			pLightQ = activeShader.getUniformLocation((pointLight + number + "].quadratic").c_str());

			glm::vec3 ambientLight = m_Lights[i]->ambient;
			glm::vec3 diffuseLight = m_Lights[i]->diffuse;
			glm::vec3 specularLight = m_Lights[i]->specular;

			glUniform3fv(lightAID, 1, glm::value_ptr(ambientLight));
			glUniform3fv(lightDID, 1, glm::value_ptr(diffuseLight));
			glUniform3fv(lightSID, 1, glm::value_ptr(specularLight));

			glm::vec3 lightPosition = m_Lights[i]->position;

			glUniform3fv(lightPosID, 1, glm::value_ptr(lightPosition));

			float constant = m_Lights[i]->constant;
			float linear = m_Lights[i]->linear;
			float quadratic = m_Lights[i]->quadratic;

			glUniform1f(pLightC, constant);
			glUniform1f(pLightL, linear);
			glUniform1f(pLightQ, quadratic);

		}




		GLuint dirLightDirID, dirLightAID, dirLightDID, dirLightSID;

		dirLightDirID = activeShader.getUniformLocation("dirLight.direction");
		dirLightAID = activeShader.getUniformLocation("dirLight.ambient");
		dirLightDID = activeShader.getUniformLocation("dirLight.diffuse");
		dirLightSID = activeShader.getUniformLocation("dirLight.specular");


		glUniform3fv(dirLightDirID, 1, glm::value_ptr(dirLight->direction));
		glUniform3fv(dirLightAID, 1, glm::value_ptr(dirLight->ambient));
		glUniform3fv(dirLightDID, 1, glm::value_ptr(dirLight->diffuse));
		glUniform3fv(dirLightSID, 1, glm::value_ptr(dirLight->specular));




		ambientID = activeShader.getUniformLocation("material.ambient");
		diffuseID = activeShader.getUniformLocation("material.diffuse");
		specularID = activeShader.getUniformLocation("material.specular");
		shininessID = activeShader.getUniformLocation("material.shininess");

	

		glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
		glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
		GLfloat shininess = 32.0f;

		glUniform3fv(ambientID, 1, glm::value_ptr(ambient));
		glUniform3fv(diffuseID, 1, glm::value_ptr(diffuse));
		glUniform3fv(specularID, 1, glm::value_ptr(specular));
		glUniform1f(shininessID, shininess);








		glDrawElementsBaseVertex(GL_TRIANGLES,
			m_Entries[i].NumIndices, GL_UNSIGNED_INT,
			(void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex),
			m_Entries[i].BaseVertex);



	}
	glBindVertexArray(0);
}
