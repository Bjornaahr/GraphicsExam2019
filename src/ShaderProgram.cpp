#include "ShaderProgram.hpp"
#include <GFX/gfx.h>

ShaderProgram::ShaderProgram(GLuint ID) : id(ID)
{
	GLint variableCount;
	GLint maxNameLength;

	//get count of active attributes
	glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &variableCount);
	// Read the length of the longest active uniform name.
	glGetProgramiv(id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxNameLength);


	std::string name;
	name.resize(maxNameLength);

	for (size_t i = 0; i < variableCount; i++)
	{
		GLint ignored;
		GLenum type;
		glGetActiveAttrib(id, i, maxNameLength, nullptr, &ignored, &type, name.data());

		addAttributeLocation(name.data());
		if(debugMode)
		GFX_DEBUG("[ATTRIBUTE] %s: \"%s\" cached aside in the active shader program!", Gfx::GLEnumToString(type), name.data());
	}

	//get count of active uniforms
	glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &variableCount);
	// Read the length of the longest active uniform name.
	glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);

	name.clear();
	name.resize(maxNameLength);
	for (size_t i = 0; i < variableCount; i++)
	{
		GLint ignored;
		GLenum type;
		glGetActiveUniform(id, i, maxNameLength, nullptr, &ignored, &type, name.data());

		addUniformLocation(name.data());
		//GFX_DEBUG("[UNIFORM] %s: \"%s\" cached aside in the active shader program!", Gfx::GLEnumToString(type), name.data());
	}
}


void
ShaderProgram::bind()
{
	glUseProgram(id);
}

void 
ShaderProgram::unbind()
{
	glUseProgram(0);
}

void 
ShaderProgram::destroy()
{
	attributes.clear();
	uniforms.clear();
	unbind();
	glDeleteProgram(id);
}

void
ShaderProgram::addUniformLocation( const char* name )
{
	uniforms.try_emplace(name, glGetUniformLocation(id, name));
}

GLint 
ShaderProgram::getUniformLocation( const char* name ) const
{
	try {
		return uniforms.at(name);
	}
	catch (const std::out_of_range& e) {
		if(debugMode)
		GFX_WARN("Trying to get location of unregistered, inactive or nonexistant uniform \"%s\"", name);

		return -1;
	}
}

void
ShaderProgram::addAttributeLocation( const char* name )
{
	attributes.try_emplace(name, glGetAttribLocation(id, name));
}

GLint 
ShaderProgram::getAttributeLocation( const char* name ) const
{
	try {
		return attributes.at(name);
	}
	catch (const std::out_of_range& e) {
		if(debugMode)
		GFX_WARN("Trying to get location of unregistered, inactive or nonexistant attribute \"%s\"", name);
		return -1;
	}
}

void ShaderProgram::setDebug(bool mode) {
	debugMode = mode;
}