#pragma

#include <mesh.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Terrain {


	struct Vertex {
		int x;
		int y;
		int z;
	};

private:


	const float SIZE = 800;
	const int VERTEX_COUNT = 128;

	int nOutputHeight = 256;
	int nOutputWidth = 256;


	float x;
	float z;
	
	int* vertecies = 0;
	int* indices = 0;

	Texture texture;
	Mesh *mesh;
	void Terrain::CreatePlane();
	int Terrain::getVertexCount();
	int Terrain::getIndexCount();

	GLuint m_Buffers[4];
	GLuint m_VAO, uboMatrices, m_lightbuffer;


public:
	Terrain(int gridX, int gridZ);
	void HeighMapGenerate(int nWidth, int nHeight, float *fSeed, int nOctaves, float *fOutput);
	void Terrain::Render(CameraMovement* cam, glm::mat4 model, DirectionalLight* dirLight, std::vector<PointLight*> pointLights);
};