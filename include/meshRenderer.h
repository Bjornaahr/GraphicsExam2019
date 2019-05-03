#pragma once
#include <gameComponent.h>
#include <mesh.h>


class MeshRenderer : public GameComponent {
private:
	Mesh *mesh;

public:
	void Render(CameraMovement *cam, std::unique_ptr<Transform> const &m_Transform, DirectionalLight* dirLight, std::vector<PointLight*> pointLights);
	void Init();
	void SetShadowShader(char vertexShader[], char fragmentShader[]);
	void SetShader(char shader[]);
	void LoadMesh(const std::string& Filename);
};