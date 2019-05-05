#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <imgui.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vector>


#define GFX_IMPLEMENTATION
//#include <mesh.h>
#include <gameobject.h>
#include <cameraMovement.h>
#include <meshRenderer.h>
#include <Terrain/Terrain.h>
#include <Terrain/HeightmapGenerator.h>




void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void SetupTerrain(char path[] = "resources/models/plane/heightMap.png");

const int WIDTH = 1600, HEIGHT = 900;
std::unique_ptr<GameObject> NewModel;
std::unique_ptr<GameObject> Dragon;
std::unique_ptr<GameObject> TestModel;

CameraMovement* Camera;
std::vector<std::unique_ptr<GameObject>> models;
MeshRenderer* renderer;
MeshRenderer* newRender;
MeshRenderer* newRender1;
std::unique_ptr<Transform> modelTransform;
DirectionalLight* dLight;
std::vector<PointLight*> m_Lights;
HeightMapGenerator* heightmapgen;
//DirectionalLight* dLight;

 

float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;
bool mouseMove = true;

bool hasGeneratedTerrain = false;

//Set up all the renderers, map, player, etc.
void setup() {







	
	glClearDepth(1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);


	dLight = new DirectionalLight{
	glm::vec3(-0.3,-1,-0.5),
	glm::vec3(0.2,0.2,0.2),
	glm::vec3(1,1,1),
	glm::vec3(0.5,0.5,0.5), };

	modelTransform = std::unique_ptr<Transform>(new Transform{ glm::vec3(0,0,0), glm::vec3(1,1,1), glm::vec3(0,0,0) });

}


void LoadNewModel(std::string modelPath, std::string modelName) {

	newRender = new MeshRenderer();

	NewModel = std::unique_ptr<GameObject>(new GameObject(modelName, false));
	NewModel->AddComponent(newRender);
	NewModel->LoadMesh(modelPath, "");
	NewModel->SetShader("shader");	
	
	models.push_back(std::move_if_noexcept(NewModel));

}


//Do physics, rendering or any state changes here
void game_loop(GLFWwindow *w, double deltaTime) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	if (hasGeneratedTerrain) {
		Camera->processInput(w, mouseMove);


		for (auto& gameObject : models) {
			gameObject->Render(Camera, dLight, m_Lights);
		}
	}

}

void SetupTerrain(char path[]) {
	heightmapgen = new HeightMapGenerator(6, 4);

	renderer = new MeshRenderer();
	newRender1 = new MeshRenderer();
	Dragon = std::unique_ptr<GameObject>(new GameObject("Terrain", true));
	TestModel = std::unique_ptr<GameObject>(new GameObject("Deer", false));

	Dragon->AddComponent(renderer);
	Dragon->LoadMesh("resources/models/plane/untitled.obj", path);
	Dragon->SetShader("test");
	models.push_back(std::move(Dragon));


	

	Camera = new CameraMovement();



	//	m_Lights.push_back(m);




	TestModel->AddComponent(newRender1);
	TestModel->LoadMesh("resources/models/deer/deer.obj", "");
	TestModel->SetShader("shader");
	models.push_back(std::move(TestModel));

	hasGeneratedTerrain = true;

}


void UI(double deltaTime, GLFWwindow *w) {

	static bool showUI = true;

	bool showLoader = true;
	bool useShader = false;
	bool showSceneGraph = true;
	bool move = true;
	bool showModelTransform = true;
	bool showLighting = true;
	bool showCameraControlls = true;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();





	if (hasGeneratedTerrain) {


		if (glfwGetKey(w, GLFW_KEY_1) == GLFW_PRESS) {
			GFX_DEBUG("%i", showUI);
			if (!showUI) {
				showUI = true;
				showLoader = false;
				useShader = false;
				showSceneGraph = false;
				move = false;
				showModelTransform = false;
				showLighting = false;
				showCameraControlls = false;
			}
			else {
				showUI = false;
				showLoader = true;
				useShader = false;
				showSceneGraph = true;
				move = true;
				showModelTransform = true;
				showLighting = true;
				showCameraControlls = true;
			}
		}


		if (showLoader) {
			ImGui::Begin("Load new Model");

			static char modelBuf[128];
			static char nameBuf[128];

			ImGui::InputText("Model Path", modelBuf, IM_ARRAYSIZE(modelBuf));
			ImGui::InputText("Model Name", nameBuf, IM_ARRAYSIZE(nameBuf));


			if (ImGui::Button("Load new Model")) {
				LoadNewModel(modelBuf, nameBuf);
			}


			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			ImGui::End();
		}

		if (useShader) {
			ImGui::Begin("Use another Shader");

			static char shader[128];

			ImGui::InputText("Shader Name", shader, IM_ARRAYSIZE(shader));

			if (ImGui::Button("Load Shader")) {
				for (auto& gameObject : models) {

					gameObject->SetShader(shader);
				}
			}

			ImGui::End();
		}



		if (showSceneGraph) {
			ImGui::Begin("SceneGraph");



			if (ImGui::TreeNode("Top"))
			{

				static int selection_mask = (1 << 2);
				int node_clicked = -1;
				ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3); // Increase spacing to differentiate leaves from expanded contents.
				int i = 0;
				for (auto& gameObject : models)
				{
					// Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
					ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << i)) ? ImGuiTreeNodeFlags_Selected : 0);

					// Node
					bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "%s", gameObject.get()->getName().c_str());
					if (ImGui::IsItemClicked()) {
						GFX_DEBUG("Item selected: %s", gameObject.get()->getName().c_str());
						node_clicked = i;
						gameObject.get()->setSelect(true);

					}
					else if (ImGui::IsMouseClicked(0) && !ImGui::IsItemClicked() && ImGui::IsMouseHoveringWindow()) {
						gameObject.get()->setSelect(false);
					}

					if (node_open)
					{

						node_clicked = gameObject.get()->GetChildren(gameObject.get()->getSelect(), (++i + 1000), node_clicked, selection_mask);
						ImGui::TreePop();
					}


					i++;
				}
				if (node_clicked != -1)
				{
					// Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
					if (ImGui::GetIO().KeyCtrl)
						selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
					else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
						selection_mask = (1 << node_clicked);           // Click to single-select
				}
				ImGui::PopStyleVar();

				ImGui::TreePop();
			}


			ImGui::End();
		}



		if (showModelTransform) {

			ImGui::Begin("Transform");



			for (auto& gameObject : models) {

				std::unique_ptr<Transform> t = std::unique_ptr<Transform>(gameObject->getTransform());

				//Check if child is selcted to make it work
				if (t != nullptr && gameObject->getSelectWChildern()) {
					modelTransform = std::move(t);

					static float translateX;
					static float translateY;
					static float translateZ;

					static float rotateX;
					static float rotateY;
					static float rotateZ;

					static float scaleX = 1;
					static float scaleY = 1;
					static float scaleZ = 1;


					translateX = modelTransform.get()->position.x;
					translateY = modelTransform.get()->position.y;
					translateZ = modelTransform.get()->position.z;

					rotateX = modelTransform.get()->rotation.x;
					rotateY = modelTransform.get()->rotation.y;
					rotateZ = modelTransform.get()->rotation.z;

					scaleX = modelTransform.get()->scale.x;
					scaleY = modelTransform.get()->scale.y;
					scaleZ = modelTransform.get()->scale.z;



					ImGui::PushItemWidth(ImGui::GetFontSize() * 15);

					ImGui::InputFloat("XT", &translateX, 0.01f, 1.0f, "%.3f");
					ImGui::SameLine();
					ImGui::InputFloat("YT", &translateY, 0.01f, 1.0f, "%.3f");
					ImGui::SameLine();
					ImGui::InputFloat("ZT", &translateZ, 0.01f, 1.0f, "%.3f");

					ImGui::InputFloat("XR", &rotateX, 0.01f, 1.0f, "%.3f");
					ImGui::SameLine();
					ImGui::InputFloat("YR", &rotateY, 0.01f, 1.0f, "%.3f");
					ImGui::SameLine();
					ImGui::InputFloat("ZR", &rotateZ, 0.01f, 1.0f, "%.3f");

					ImGui::InputFloat("XS", &scaleX, 0.01f, 1.0f, "%.3f");
					ImGui::SameLine();
					ImGui::InputFloat("YS", &scaleY, 0.01f, 1.0f, "%.3f");
					ImGui::SameLine();
					ImGui::InputFloat("ZS", &scaleZ, 0.01f, 1.0f, "%.3f");

					modelTransform.get()->position.x = translateX;
					modelTransform.get()->position.y = translateY;
					modelTransform.get()->position.z = translateZ;

					modelTransform.get()->rotation.x = rotateX;
					modelTransform.get()->rotation.y = rotateY;
					modelTransform.get()->rotation.z = rotateZ;

					modelTransform.get()->scale.x = scaleX;
					modelTransform.get()->scale.y = scaleY;
					modelTransform.get()->scale.z = scaleZ;

					gameObject->Update(std::move(modelTransform));
				}
			}

			ImGui::End();
		}

		if (showLighting) {



			ImGui::Begin("Lighting panel");

			static char nameBuf[128] = "Plz give me a name";
			ImGui::InputText("", nameBuf, IM_ARRAYSIZE(nameBuf));
			ImGui::SameLine();
			if (ImGui::Button("Add new pointlight")) {
				//Creates a new pointlight
				PointLight* m = new PointLight{
				glm::vec3(0,0,0),
				glm::vec3(0.2,0.2,0.2),
				glm::vec3(0,1,1),
				glm::vec3(0.5,0.5,0.5),
				1,
				0.009,
				0.0032,
				nameBuf,
				};

				m_Lights.push_back(m);

			}

			if (ImGui::TreeNode("Directional Light")) {

				static float dirX = dLight->direction.x, dirY = dLight->direction.y, dirZ = dLight->direction.z;

				ImGui::PushItemWidth(ImGui::GetFontSize() * 10);
				ImGui::InputFloat("X", &dirX, 0.1f, 1.0f, "%.1f");
				ImGui::SameLine();
				ImGui::InputFloat("Y", &dirY, 0.1f, 1.0f, "%.1f");
				ImGui::SameLine();
				ImGui::InputFloat("Z", &dirZ, 0.1f, 1.0f, "%.1f");


				static ImVec4 Ambcolor = ImVec4(50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 255.0f / 255.0f);
				static ImVec4 Diffcolor = ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
				static ImVec4 Specolor = ImVec4(125.0f / 255.0f, 125.0f / 255.0f, 125.0f / 255.0f, 255.0f / 255.0f);;

				ImGui::ColorEdit3("Ambient##1", (float*)&Ambcolor, ImGuiColorEditFlags_Float);
				ImGui::ColorEdit3("Diffuse##1", (float*)&Diffcolor, ImGuiColorEditFlags_Float);
				ImGui::ColorEdit3("Specular##1", (float*)&Specolor, ImGuiColorEditFlags_Float);

				dLight->direction = glm::vec3(dirX, dirY, dirZ);
				dLight->ambient = glm::vec3(Ambcolor.x, Ambcolor.y, Ambcolor.z);
				dLight->diffuse = glm::vec3(Diffcolor.x, Diffcolor.y, Diffcolor.z);
				dLight->specular = glm::vec3(Specolor.x, Specolor.y, Specolor.z);
				ImGui::TreePop();
			}

			int i = 0;
			for (PointLight* light : m_Lights) {
				if (ImGui::TreeNode(light->name.c_str())) {


					//Floats needed to make light
					static float posX, posY, posZ, constant = 1, linear = 0.09f, quadratic = 0.032f;

					//Set values to the actual value of the light
					posX = light->position.x;
					posY = light->position.y;
					posZ = light->position.z;
					constant = light->constant;
					linear = light->linear;
					quadratic = light->quadratic;


					//Position of light ImGUI
					ImGui::PushItemWidth(ImGui::GetFontSize() * 10);
					ImGui::InputFloat("X", &posX, 0.1f, 1.0f, "%.1f");
					ImGui::SameLine();
					ImGui::InputFloat("Y", &posY, 0.1f, 1.0f, "%.1f");
					ImGui::SameLine();
					ImGui::InputFloat("Z", &posZ, 0.1f, 1.0f, "%.1f");

					//Light values
					static ImVec4 Ambcolor = ImVec4(50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 255.0f / 255.0f);
					static ImVec4 Diffcolor = ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
					static ImVec4 Specolor = ImVec4(125.0f / 255.0f, 125.0f / 255.0f, 125.0f / 255.0f, 255.0f / 255.0f);;

					//Set the light value to be the same as the acctual light
					Ambcolor = ImVec4(light->ambient.x, light->ambient.y, light->ambient.z, 1);
					Diffcolor = ImVec4(light->diffuse.x, light->diffuse.y, light->diffuse.z, 1);
					Specolor = ImVec4(light->specular.x, light->specular.y, light->specular.z, 1);

					//Imgui input
					ImGui::ColorEdit3("Ambient##1", (float*)&Ambcolor, ImGuiColorEditFlags_Float);
					ImGui::ColorEdit3("Diffuse##1", (float*)&Diffcolor, ImGuiColorEditFlags_Float);
					ImGui::ColorEdit3("Specular##1", (float*)&Specolor, ImGuiColorEditFlags_Float);
					ImGui::InputFloat("Constant", &constant, 0.001f, 1.0f, "%.4f");
					ImGui::InputFloat("Linear", &linear, 0.001f, 1.0f, "%.4f");
					ImGui::InputFloat("Quadratic", &quadratic, 0.001f, 1.0f, "%.4f");

					//Set the value of Input from ImGui to light
					m_Lights[i]->position = glm::vec3(posX, posY, posZ);
					m_Lights[i]->ambient = glm::vec3(Ambcolor.x, Ambcolor.y, Ambcolor.z);
					m_Lights[i]->diffuse = glm::vec3(Diffcolor.x, Diffcolor.y, Diffcolor.z);
					m_Lights[i]->specular = glm::vec3(Specolor.x, Specolor.y, Specolor.z);
					m_Lights[i]->constant = constant;
					m_Lights[i]->linear = linear;
					m_Lights[i]->quadratic = quadratic;
					ImGui::TreePop();
				}
				i++;
			}




			ImGui::End();
		}

		if (showCameraControlls) {

			static float camPosX, camPosY, camPosZ, pitch, yaw, speed = Camera->cameraSpeed;

			ImGui::Begin("Camera UI");

			glm::vec3 camPos = Camera->getPosition();

			camPosX = camPos.x;
			camPosY = camPos.y;
			camPosZ = camPos.z;
			pitch = Camera->getPitch();
			yaw = Camera->getYaw();


			ImGui::InputFloat("Camera X", &camPosX, 0.1f, 1.0f, "%.1f");
			ImGui::InputFloat("Camera Y", &camPosY, 0.1f, 1.0f, "%.1f");
			ImGui::InputFloat("Camera Z", &camPosZ, 0.1f, 1.0f, "%.1f");

			ImGui::InputFloat("Pitch", &pitch, 0.1f, 1.0f, "%.1f");
			ImGui::InputFloat("Yaw", &yaw, 0.1f, 1.0f, "%.1f");

			ImGui::InputFloat("Speed", &speed, 0.1f, 1.0f, "%.1f");

			Camera->cameraSpeed = speed;

			if (ImGui::Button("Change camera movment")) {
				if (mouseMove) {
					mouseMove = false;
				}
				else mouseMove = true;
			}


			if (!mouseMove) {
				Camera->setPosition(glm::vec3(camPosX, camPosY, camPosZ));
				Camera->setPitch(pitch);
				Camera->setYaw(yaw);
			}


			ImGui::End();
		}
	}




	else {

		ImGui::Begin("Camera UI");

		static char Buffer[128];
		ImGui::InputText("Hieghtmap path", Buffer, IM_ARRAYSIZE(Buffer));
		ImGui::SameLine();
		if (ImGui::Button("Load heightmap")) {
			SetupTerrain(Buffer);
		}


	

		if (ImGui::Button("Randomly generate terrain")) {
			SetupTerrain();
		}


		ImGui::End();
	}




	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}



int main(void) {
	// Initialise GLFW
	if (!glfwInit()) {
		GFX_ERROR("Failed to initialize GLFW\n");
	}
	const char* glsl_version = "#version 330";
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Handin2", NULL, NULL);
	if (window == NULL) {
		GFX_ERROR("Failed to open GLFW window.\n");
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		GFX_ERROR("Failed to initialize GLEW\n");
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glfwSwapInterval(0);

	// Set OpenGL options

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init(glsl_version);
	io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard;




	setup();
	glClearColor(1.0f, 0.0f, 0.8f, 1);

	double lastTime = glfwGetTime();
	int nbFrames = 0;
	double deltaTime = 0;
	double oldTime = 0;

	do {
		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		deltaTime = currentTime - oldTime;
		game_loop(window, deltaTime);
		UI(deltaTime, window);
		glfwSwapBuffers(window);
		glfwPollEvents();
		oldTime = currentTime;
	} while (glfwWindowShouldClose(window) == 0);
	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}


