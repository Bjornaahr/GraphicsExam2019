#pragma once
#include <string>
#include <cameraMovement.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <mesh.h>

struct Transform {
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
};


class GameComponent {
private:


public:
	virtual void setGameObject();
	virtual void Init();
	virtual void Input();
	virtual void Update();
	virtual void Render(CameraMovement *cam, std::unique_ptr<Transform> const &m_Transform, DirectionalLight* dirLight, std::vector<PointLight*> pointLights, bool isTerrain);
	virtual void SetShadowShader(char vertexShader[], char fragmentShader[]);
	virtual void SetShader(char shader[]);
	virtual void LoadMesh(const std::string& Filename, char path[]);
};