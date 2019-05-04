#include <Terrain/HeightmapGenerator.h>
#include <fstream>
#include <stb_image_aug.h>

HeightMapGenerator::HeightMapGenerator() {



	unsigned char *heightmap = new unsigned char[3];


	unsigned char r, g, b;

	r = 0;
	g = 0;
	b = 0;


	for (int y = 0; y < 128; y++) {
		for (int x = 0; x < 128; x++) {

			int r, g, b;

			if (y % 2 == 0) {
				r = 255; g = 255; b = 255;
			}
			else { r = 0; g = 0; b = 0; }
			
	
		}


	}


	stbi_write_bmp("resources/models/plane/heightMap.png", 1, 1, 3, heightmap);
}