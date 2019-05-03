#include "textureManager.h"
#include "SOIL.h"


std::map<std::string, Texture>    TextureManager::Textures;

Texture TextureManager::LoadTexture(const GLchar *file, std::string name)
{
	//Loads picture from file and stores it for future reference
	Textures[name] = loadTextureFromFile(file);
	return Textures[name];
}
//Returns the requested texture
Texture TextureManager::GetTexture(std::string name)
{
	return Textures[name];
}

//Deletes all textures
void TextureManager::Clear()
{
	// Deletes all textures
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

Texture TextureManager::loadTextureFromFile(const GLchar *file) {

	int width, height;
	unsigned char* image;

	Texture texture;
	//Loads image from file
	image = SOIL_load_image(file, &width, &height, 0, SOIL_LOAD_AUTO);
	//Generates image
	texture.Generate(width, height, image);
	//Clears image
	SOIL_free_image_data(image);
	return texture;
}