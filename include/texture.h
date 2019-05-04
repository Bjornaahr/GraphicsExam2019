#pragma once
#include <GL/glew.h>
#include <string>
#include <GFX/gfx.h>

class Texture {
public:
	//Refrence to texture object
	GLuint ID;
	//Dimensions of texture
	GLuint Width, Height;

	Texture(GLenum TextureTarget, const std::string& file);
	Texture();
	//Binds texture to active GL_TEXTURE_2D object
	void Bind(GLenum TextureUnit) const;
	void BindShadow() const;
	GLenum TextureSpot;

};