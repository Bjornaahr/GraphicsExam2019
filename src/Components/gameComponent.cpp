#include <gameComponent.h>

void GameComponent::setGameObject(){}
void GameComponent::Init(){}
void GameComponent::Input(){}
void GameComponent::Update(){}
void GameComponent::Render(CameraMovement *cam, std::unique_ptr<Transform> const &m_Transform, DirectionalLight* dirLight, std::vector<PointLight*> pointLights){}
void GameComponent::SetShadowShader(char vertexShader[], char fragmentShader[]){}
void GameComponent::SetShader(char shader[]){}
void GameComponent::LoadMesh(const std::string& Filename){}