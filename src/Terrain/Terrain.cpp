#include <Terrain/Terrain.h>



Terrain::Terrain(int gridX, int gridZ) {
	x = (float)gridX;
	z = (float)gridZ;
	mesh = new Mesh();

	CreatePlane();

}

void Terrain::HeighMapGenerate(int nWidth, int nHeight, float *fSeed, int nOctaves, float *fOutput) {

	for (int x = 0; x < nWidth; x++) {
		for (int y = 0; y < nHeight; x++) {

			float fNoise = 0.0f;
			float fScale = 1.0f;
			float fScaleAcc = 0.0f;

			for (int i = 0; i < nOctaves; i++)
			{
				int nPitch = nWidth >> i;

				int nSampleX1 = (x / nPitch) * nPitch;
				int nSampleX2 = (x + nPitch) % nWidth;

				int nSampleY1 = (x / nPitch) * nPitch;
				int nSampleY2 = (x + nPitch) % nWidth;

				float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
				float fBlendY = (float)(x - nSampleY1) / (float)nPitch;


				float fSampleT = (1.0f, -fBlendX) * fSeed[nSampleX1] + fBlendX * fSeed[nSampleX2];
				float fSampleB = (1.0f, -fBlendX) * fSeed[nSampleY1] + fBlendX * fSeed[nSampleY2];

				fNoise += (fBlendY *(fSampleB - fSampleT) + fSampleT) * fScale;
				fScaleAcc += fScale;

				fScale = fScale / 2.0f;

			}
			//Scale to seed range
			fOutput[y * nWidth + x] = fNoise;
		}
	}

}



void Terrain::CreatePlane() {

	//vertecies = new int[3];
	//int i = 0;

	//GFX_DEBUG("%i", getVertexCount());


	//for (int row = 0; row < z; row++) {
	//		for (int col = 0; col < x; col++) {
	//			vertecies[i++] = col;
	//			vertecies[i++] = 0;
	//			vertecies[i++] = row;
	//		}
	//	
	//}


	//indices = new int[getIndexCount()];
	//i = 0;
	//

	//for (int row = 0; row < z - 1; row++) {
	//	if ((row % 1) == 0) { // even rows
	//		for (int col = 0; col < x; col++) {
	//			indices[i++] = col + row * x;
	//			indices[i++] = col + (row + 1) * x;
	//		}
	//	}
	//	else { // odd rows
	//		for (int col = x - 1; col > 0; col--) {
	//			indices[i++] = col + (row + 1) * x;
	//			indices[i++] = col - 1 + +row * x;
	//		}
	//	}
	//}





			

		int count = VERTEX_COUNT * VERTEX_COUNT;
		float *vertices = new float[count * 3];
		float *normals = new float[count * 3];
		float *textureCoords = new float[count * 2];
		int *indices = new int[6 * (VERTEX_COUNT - 1)*(VERTEX_COUNT - 1)];


		int vertexPointer = 0;
		for (int i = 0; i < VERTEX_COUNT; i++) {
			for (int j = 0; j < VERTEX_COUNT; j++) {
				vertices[vertexPointer * 3] = (float)j / ((float)VERTEX_COUNT - 1) * SIZE;
				vertices[vertexPointer * 3 + 1] = 0;
				vertices[vertexPointer * 3 + 2] = (float)i / ((float)VERTEX_COUNT - 1) * SIZE;
				normals[vertexPointer * 3] = 0;
				normals[vertexPointer * 3 + 1] = 1;
				normals[vertexPointer * 3 + 2] = 0;
				textureCoords[vertexPointer * 2] = (float)j / ((float)VERTEX_COUNT - 1);
				textureCoords[vertexPointer * 2 + 1] = (float)i / ((float)VERTEX_COUNT - 1);
				vertexPointer++;
			}
		}
		int pointer = 0;
		for (int gz = 0; gz < VERTEX_COUNT - 1; gz++) {
			for (int gx = 0; gx < VERTEX_COUNT - 1; gx++) {
				int topLeft = (gz*VERTEX_COUNT) + gx;
				int topRight = topLeft + 1;
				int bottomLeft = ((gz + 1)*VERTEX_COUNT) + gx;
				int bottomRight = bottomLeft + 1;
				indices[pointer++] = topLeft;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = topRight;
				indices[pointer++] = topRight;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = bottomRight;
			}
		}


		mesh->LoadTerrain(vertices, textureCoords, normals, indices);

}



int Terrain::getVertexCount() {
	return x * z * 3;
}

int Terrain::getIndexCount() {
	return (x * z) + (x - 1)*(z - 2);
}




void Terrain::Render(CameraMovement* cam, glm::mat4 model, DirectionalLight* dirLight, std::vector<PointLight*> pointLights) {
	


	mesh->RenderTerrain();



}