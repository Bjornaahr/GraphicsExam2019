#include <meshRenderer.h>
#include <glm/glm.hpp>

void MeshRenderer::Init() {

}

void MeshRenderer::Render(CameraMovement *cam, std::unique_ptr<Transform> const &m_Transform, DirectionalLight* dirLight, std::vector<PointLight*> pointLights){
	float rot = 0;


	glm::mat4 modelMatrix(1.f);
	glm::mat4 translate = glm::translate(modelMatrix, m_Transform.get()->position);
	glm::mat4 rotateX = glm::rotate(modelMatrix, m_Transform.get()->rotation.x, glm::vec3(1, 0, 0));
	glm::mat4 rotateY = glm::rotate(modelMatrix, m_Transform.get()->rotation.y, glm::vec3(0, 1, 0));
	glm::mat4 rotateZ = glm::rotate(modelMatrix, m_Transform.get()->rotation.z, glm::vec3(0, 0, 1));

	glm::mat4 rotateM = rotateX * rotateY * rotateZ;
	glm::mat4 scale = glm::scale(modelMatrix, m_Transform.get()->scale);

	glm::mat4 model = translate * rotateM * scale;
	mesh->Render(cam, model, dirLight, pointLights);
}

void MeshRenderer::SetShadowShader(char vertexShader[], char fragmentShader[]) {
	mesh->SetShadowShader(vertexShader, fragmentShader);
}
void MeshRenderer::SetShader(char shader[]) {
	mesh->SetShader(shader);
}

void MeshRenderer::LoadMesh(const std::string& Filename) {
	mesh = new Mesh();
	mesh->LoadMesh(Filename);
}