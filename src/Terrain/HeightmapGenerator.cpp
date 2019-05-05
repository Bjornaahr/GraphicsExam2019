#include <Terrain/HeightmapGenerator.h>
#include <fstream>
#include <stb_image_aug.h>
#include <time.h> 


HeightMapGenerator::HeightMapGenerator() {

	int size = 512;

	unsigned char *heightmap = new unsigned char[(size *size)*3];


	unsigned char r, g, b;

	
	int nOctaves = 3;
	time_t t;


	srand(time(&t));
	float fSeed = rand();


	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			
			r = 0; g = 0; b = 0;
		

			//TODO PERLIN NOISE
			float fNoise = 0.0f;
			float fScaleAcc = 0.0f;
			float fScale = 1.0f;

			for (int o = 0; o < nOctaves; o++)
			{
				
			}



	

			fSeed = rand();

			
			heightmap[(y * size + x) * 3 + 0] = (int)fSeed % 255;
			heightmap[(y * size + x) * 3 + 1] = (int)fSeed % 255;
			heightmap[(y * size + x) * 3 + 2] = (int)fSeed % 255;
			
			
	
		}


	}


	stbi_write_bmp("resources/models/plane/heightMap.png", size, size, 3, heightmap);
}


float HeightMapGenerator::FBM() {
	return 1.0f;
}
float HeightMapGenerator::noise(glm::vec2 st) {
	return 1.0f;
}