#include "texture.h"
#include <SOIL.h>
#include <stb_image_aug.h>

Texture::Texture(GLenum TextureTarget, const std::string& file) {

	GFX_DEBUG("Loading image: %s", file.c_str())
	int width, height;
	//Loads image from file
	auto image = stbi_load(file.c_str(), &width, &height, 0, 4);


	if (!image) {
		GFX_WARN("Could not load texture: %s", file.c_str());
		GFX_WARN("Reason for failure: %s", stbi_failure_reason());

		image =stbi_load("resources/textures/error/error.png", &width, &height, nullptr, 4);
	}

	glGenTextures(1, &this->ID);
	//Binds texture to this object
	GFX_DEBUG("Texture ID: %i", this->ID);
	glBindTexture(TextureTarget, this->ID);

	glTexImage2D(TextureTarget, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	//Set all texture parameters
	glTexParameteri(TextureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(TextureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(TextureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(TextureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(TextureTarget);

	//Unbinds texture
	glBindTexture(TextureTarget, 0);

	//Clears image
	stbi_image_free(image);
}

Texture::Texture() {
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	glGenTextures(1, &this->ID);
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
}


//Binds texture
void Texture::Bind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->ID);
}

