#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>

#include <GFX/gfx.h>

#include "ShaderProgram.hpp"

// @ref Inspired by https://stackoverflow.com/a/2602060
//    and http://insanecoding.blogspot.no/2011/11/how-to-read-in-file-in-c.html
/// <summary>
/// reads the <paramref name="file"/> into <paramref name="outString"/>
/// </summary>
/// <param name="file">the path of the file to read</param>
/// <param name="outString">the string to be filled</param>
/// <returns> true when successful, false when unsuccessful </returns>
auto fileToString(const std::string& file, std::string* outString) -> bool
{
	std::ifstream infile(file);

	if (!infile) {
		return false;
	}
	infile.seekg(0, std::ios::end);
	outString->resize(infile.tellg());
	infile.seekg(0, std::ios::beg);
	infile.read(outString->data(), outString->size());
	infile.close();
	return true;
}

/// <summary>
/// creates a new shader object of type <paramref name="shaderType"/>, 
/// and reads the shader code "<paramref name="src"/>" into the shader object 
/// </summary>
/// <param name="src">the source code</param>
/// <param name="shaderType">the shaderType</param>
/// <returns> the id of the created shader object </returns>
auto createShader(const char* src, GLenum shaderType) -> GLuint
{
	// Create an empty shader object
	GLuint shaderID = glCreateShader(shaderType);

	//set the shader source code of the shader object
	glShaderSource(shaderID, 1, &src, nullptr);

	// Compile the shader
	glCompileShader(shaderID);

	GLint compileOK;		//validate shader compilation
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileOK);
	if (!compileOK) {
		std::string str;
		int length;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);

		str.resize(length); 
		glGetShaderInfoLog(shaderID, length, nullptr, str.data());
		GFX_WARN("%s shader failed to compile with the error: %s\n", Gfx::GLEnumToString(shaderType), str.c_str());
		return 0;
	}
	return shaderID;
}

/// <summary>
/// creates a new shader program and its shaders based on the source files given
/// </summary>
/// <param name="vertFile">the path of the vertex shader file</param>
/// <param name="fragFile">the path of the fragment shader file</param>
/// <returns> the id of the created shader program </returns>
auto createProgram(std::string vertFile, std::string fragFile) -> ShaderProgram
{
	GLuint program = glCreateProgram();
	GLuint vertexShader = 0;
	GLuint fragmentShader = 0;

	std::string sourceStr;

	// Load and compile the vertex and fragment shaders
	if (fileToString(vertFile, &sourceStr)) {
		if (vertexShader = createShader(sourceStr.c_str(), GL_VERTEX_SHADER); !vertexShader) {
			GFX_ERROR(
				"Program creation failed because the vertex shader was invalid:\n\t%s", vertFile
			);
		}
	} else GFX_ERROR("the file \"%s\" could not be found!", vertFile.c_str());
	sourceStr.clear();
	if (fileToString(fragFile, &sourceStr)) {
		if (fragmentShader = createShader(sourceStr.c_str(), GL_FRAGMENT_SHADER); !fragmentShader) {
			GFX_ERROR(
				"Program creation failed because the fragment shader was invalid:\n\t%s", fragFile
			);
		}
	} else GFX_ERROR("the file \"%s\" could not be found!", fragFile.c_str());

	// Create a program object and attach the two shaders we have compiled, the program object contains
	// both vertex and fragment shaders as well as information about uniforms and attributes common to both.

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);


	// Now that the fragment and vertex shader has been attached, 
	// we no longer need these two separate objects and should delete them to free the memory.
	// The attachment to the shader program will keep them alive, as long as we keep the shaderProgram.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Link the different shaders that are bound to this program, this creates a final shader that 
	// we can use to render geometry with.
	glLinkProgram(program);
	
	GLint linkOK;
	glGetProgramiv(program, GL_LINK_STATUS, &linkOK);
	
	// Verify that the program linked
	if (!linkOK) {
		std::string str;
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		str.resize(length);

		glGetProgramInfoLog(program, length, nullptr, str.data());
		GFX_ERROR("The program consisting of \"%s\" and  \"%s\" failed to link: %s\n", vertFile, fragFile, str.c_str());
	}
	glUseProgram(program);

	return ShaderProgram(program);
}
