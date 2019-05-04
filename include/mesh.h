#pragma once


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <vector>
#include <assimp/postprocess.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <texture.h>
#include <GFX/gfx.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cameraMovement.h>


struct PointLight {
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular; //Takes 4*sizeof(vec4)

	float constant;
	float linear;
	float quadratic; //Takes 3*sizeof(float)
	std::string name;
};

struct DirectionalLight {
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};


struct Vertex
{
	glm::vec3 m_pos;
	glm::vec2 m_tex;
	glm::vec3 m_normal;

	Vertex() {}

	Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = normal;
	}
};

class Mesh {
public:
	Mesh();
	~Mesh();

	bool LoadMesh(const std::string& Filename);
	void LoadTerrain(float *vertecies, float *textureCoords, float *normals, int *indices);
	void RenderTerrain();

	glm::mat4 ShadowRenderer();
	void Render(CameraMovement* cam, glm::mat4 model, DirectionalLight* dirLight, std::vector<PointLight*> pointLights, bool terrain);
	void SetShadowShader(char vertexShader[], char fragmentShader[]);
	void SetShader(char shader[]);


private:
	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(const aiMesh* paiMesh,
		std::vector<glm::vec3> &Positions,
		std::vector<glm::vec3> &Normals,
		std::vector<glm::vec2> &TexCoords,
		std::vector<unsigned int> &Indices);
	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF

#define INDEX_BUFFER 0 
#define POS_VB 1
#define NORMAL_VB 2
#define TEXCOORD_VB 3

	GLuint m_VAO, uboMatrices, m_lightbuffer;
	GLuint m_Buffers[4];
	int NR_LIGHTS = 1;


	struct MeshEntry {
		MeshEntry() {
			NumIndices = 0;
			BaseVertex = 0;
			BaseIndex = 0;
			MaterialIndex = INVALID_MATERIAL;
		}

		~MeshEntry();
		unsigned int BaseVertex;
		unsigned int BaseIndex;
		unsigned int NumIndices;
		unsigned int MaterialIndex;
	};

	std::vector<MeshEntry> m_Entries;
	std::vector<Texture*> m_Textures;
	Texture* HeightMap;
	Texture* depthMap;
	Texture* SandTexture;
	Texture* GrassTexture;
	Texture* StoneTexture;
	Texture* SnowTexture;

	std::vector<PointLight*> m_Lights;
	DirectionalLight* m_dirLight;
};
