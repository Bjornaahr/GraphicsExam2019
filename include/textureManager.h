#pragma once

#include "texture.h"
#include <map>

class TextureManager {
public:

	static std::map<std::string, Texture> Textures;

	static Texture LoadTexture(const GLchar *file, std::string name);

	//Retrieves a stored texture
	static Texture GetTexture(std::string name);

	//Deletes textures
	static void Clear();
private:
	TextureManager() { }
	//Loads texture from file
	static Texture loadTextureFromFile(const GLchar *file);

};