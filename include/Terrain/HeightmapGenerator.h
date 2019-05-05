#pragma once

#include <stb_image_aug.h>
#include <glm/glm.hpp>
#include <PerlinNoise.hpp>


class HeightMapGenerator {
public:
	HeightMapGenerator(int octaves, double frequency);
private:
	void HeightMapGenerator::PerlinNoise(int octaves, double frequency);
	float noise(glm::vec2 st);
	int size;
	siv::PerlinNoise perlin;
	double** m_data = nullptr;
};