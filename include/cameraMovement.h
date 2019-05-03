#pragma once


#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GFX/gfx.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CameraMovement {
public:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	float deltaTime;
	float lastFrame;
	double xpos;
	double ypos;
	bool firstMouse;
	float lastX, lastY;
	float yaw, pitch;

	CameraMovement();
	void processInput(GLFWwindow *window);
};