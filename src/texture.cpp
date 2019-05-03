#include "texture.h"
#include <SOIL.h>

Texture::Texture(GLenum TextureTarget, const std::string& file) {

	int width, height;
	unsigned char* image;
	//Loads image from file
	image = SOIL_load_image(file.c_str(), &width, &height, 0, SOIL_LOAD_AUTO);


	if (!image) {

		GFX_WARN("Could not load texture: %s", file.c_str());
		image = SOIL_load_image("resources/textures/error/error.png", &width, &height, 0, SOIL_LOAD_AUTO);
	}

	glGenTextures(1, &this->ID);
	//Binds texture to this object
	glBindTexture(TextureTarget, this->ID);

	glTexImage2D(TextureTarget, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	//Set all texture parameters
	glTexParameteri(TextureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(TextureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(TextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(TextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(TextureTarget);

	//Unbinds texture
	glBindTexture(TextureTarget, 0);

	//Clears image
	SOIL_free_image_data(image);
}

Texture::Texture() {
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	glGenTextures(1, &this->ID);
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
}


//Binds texture
void Texture::Bind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->ID);
}

