#include <Terrain/HeightmapGenerator.h>
#include <fstream>
#include <stb_image_aug.h>

HeightMapGenerator::HeightMapGenerator() {

	int size = 512;

	unsigned char *heightmap = new unsigned char[(size *size)*3];


	unsigned char r, g, b;

	r = 0;
	g = 0;
	b = 0;


	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {

			if (y % 8 == 0) {
				r = 255; g = 255; b = 255;

				heightmap[(y * size + x) * 3 + 0] = r;
				heightmap[(y * size + x) * 3 + 1] = g;
				heightmap[(y * size + x) * 3 + 2] = b;


			}
			else { 
			r = 0; g = 0; b = 0;
			
			heightmap[(y * size + x) * 3 + 0] = r;
			heightmap[(y * size + x) * 3 + 1] = g;
			heightmap[(y * size + x) * 3 + 2] = b;
			}
			
	
		}


	}


	stbi_write_bmp("resources/models/plane/heightMap.png", size, size, 3, heightmap);
}