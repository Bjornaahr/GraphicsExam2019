#include <Terrain/HeightmapGenerator.h>
#include <fstream>
#include <stb_image_aug.h>
#include <time.h> 
#include <GFX/gfx.h>


HeightMapGenerator::HeightMapGenerator(int octaves, double frequency) {

	size = 1024;

	m_data = new double*[size];
	for (int h = 0; h < size; h++)
	{
		m_data[h] = new double[size];

		for (int w = 0; w < size; w++)
		{
			m_data[h][w] = 0;
		}
	}

	unsigned char *heightmap = new unsigned char[(size *size)*3];
	PerlinNoise(octaves, frequency);


	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
						
			
			heightmap[(y * size + x) * 3 + 0] = (int)round(m_data[x][y] * 255.f);
			heightmap[(y * size + x) * 3 + 1] = (int)round(m_data[x][y] * 255.f);
			heightmap[(y * size + x) * 3 + 2] = (int)round(m_data[x][y] * 255.f);
			
			
	
		}


	}


	stbi_write_bmp("resources/models/plane/heightMap.png", size, size, 3, heightmap);
}


void HeightMapGenerator::PerlinNoise(int octaves, double frequency) {

	srand(time(NULL));

	perlin = siv::PerlinNoise(rand());
	
	double nx = size / frequency, ny = size / frequency;
	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			m_data[y][x] = perlin.octaveNoise0_1(x / nx, y / ny, octaves);

		}
	}




}
float HeightMapGenerator::noise(glm::vec2 st) {
	return 1.0f;
}