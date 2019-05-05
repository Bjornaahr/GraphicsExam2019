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
	void processInput(GLFWwindow *window, bool mouseMove);

	glm::vec3 CameraMovement::getPosition();

	void CameraMovement::setPosition(glm::vec3 pos);

	float getPitch();
	float getYaw();

	void CameraMovement::setYaw(float Ya);
	void CameraMovement::setPitch(float Pit);

	float cameraSpeed = 22.5;
};