#pragma once

#include <stb_image_aug.h>
#include <glm/glm.hpp>

class HeightMapGenerator {
public:
	HeightMapGenerator();
private:
	float FBM();
	float noise(glm::vec2 st);
};