#pragma once

#include <vector>
#include <glm.hpp>
#include <cameraMovement.h>
#include <gameComponent.h>

class GameObject
{
private:
	std::vector<std::unique_ptr<GameObject>> children;
	std::vector<GameComponent*> components;
	std::string Name;
	bool selected = false;
	std::unique_ptr<Transform> m_Transform;
	bool Terrain;

public:
	GameObject(std::string name, bool isTerrain);
	void Update(std::unique_ptr<Transform> transform);
	void Input();
	void AddChild(std::unique_ptr<GameObject> child);
	void AddComponent(GameComponent *component);
	void LoadMesh(const std::string& Filename, char path[]);
	void SetShadowShader(char vertexShader[], char fragmentShader[]);
	void SetShader(char shader[]);
	void Render(CameraMovement* cam, DirectionalLight* dirLight, std::vector<PointLight*> pointLights);
	std::string getName();
	int GetChildren(bool select, int i, int node_clicked, int selection_mask);
	void setSelect(bool set);
	bool getSelect();
	bool getSelectWChildern();
	std::unique_ptr<Transform> getTransform();
	GameObject* Search(std::string name);
	bool SearchChild(std::string name);
	void moveTransform();
	void setTransform(glm::vec3 pos);
};