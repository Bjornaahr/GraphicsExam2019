#include <gameobject.h>
#include <imgui.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <algorithm>

GameObject::GameObject(std::string name, bool isTerrain) {
	Name = name;
	m_Transform = std::unique_ptr<Transform>(new Transform{ glm::vec3(0,0,0), glm::vec3(1,1,1), glm::vec3(0,0,0) });
    Terrain = isTerrain;
}


void GameObject::Update(std::unique_ptr<Transform> transform) {

	Transform tran = *transform.get();


	if (selected) {
		m_Transform = std::move(transform);
	}

	
		for (auto& child : children) {
			std::unique_ptr<Transform> t = std::make_unique<Transform>(tran);
			child->Update(std::move(t));
		}

		for (GameComponent *component : components) {
			component->Update();
		}
	
}


void GameObject::Input() {

	for (auto& child : children) {
		child->Input();
	}

	for (GameComponent *component : components) {
		component->Input();
	}

}


void GameObject::AddChild(std::unique_ptr<GameObject> child) {
	if (selected) {
		children.push_back(std::move(child));
	}
	else {
		for (auto& child : children) {
			child->AddChild(std::move(child));
		}
	}
}

void GameObject::AddComponent(GameComponent *component) {
	components.push_back(component);
}

void GameObject::LoadMesh(const std::string& Filename) {
	for (GameComponent *component : components) {
		component->LoadMesh(Filename);
	}
}

void GameObject::SetShadowShader(char vertexShader[], char fragmentShader[]) {
	for (GameComponent *component : components) {
		component->SetShadowShader(vertexShader, fragmentShader);
	}

	for (auto& child : children) {
		child->SetShadowShader(vertexShader, fragmentShader);
	}

}

void GameObject::SetShader(char shader[]) {
	for (GameComponent *component : components) {
		component->SetShader(shader);
	}

	for (auto& child : children) {
		child->SetShader(shader);
	}

}

void GameObject::Render(CameraMovement* cam, DirectionalLight* dirLight, std::vector<PointLight*> pointLights) {


	for (GameComponent *component : components) {
		//std::unique_ptr<Transform> t = std::unique_ptr<Transform>(m_Transform.get());
		component->Render(cam, m_Transform, dirLight, pointLights, Terrain);
	}

	for (auto& child : children) {
		child->Render(cam, dirLight, pointLights);
	}
}

std::string GameObject::getName() {
	return Name;
}

int GameObject::GetChildren(bool select, int i, int node_clicked, int selection_mask) {

	for (auto& child : children)
	{
		// Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << i)) ? ImGuiTreeNodeFlags_Selected : 0);
	
			// Node
			bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "%s", child.get()->getName().c_str());
			if (ImGui::IsItemClicked()) {
				GFX_DEBUG("Item selected: %s", child.get()->getName().c_str());
				node_clicked = i;
				child.get()->setSelect(true);
			}	else if(ImGui::IsMouseClicked(0) && !ImGui::IsItemClicked() && ImGui::IsMouseHoveringWindow()){
				child.get()->setSelect(false);
			}
			
			if (node_open)
			{
				child.get()->GetChildren(ImGui::IsItemClicked(), (++i + 1000), node_clicked, selection_mask);
				ImGui::TreePop();
			}
		


		if (select) {
			child.get()->setSelect(true);
		}
		
		i++;
	}
	
	return node_clicked;
}


void GameObject::setSelect(bool set) {
	selected = set;
}

bool GameObject::getSelect() {
	return selected;
}

bool GameObject::getSelectWChildern() {
	
	if (selected) {
		return selected;
	}
	
		for (auto& child : children) {
			return child->getSelectWChildern();
		}
	


}

std::unique_ptr<Transform> GameObject::getTransform(){

	if (m_Transform != nullptr) {
		if (selected) {
			//GFX_DEBUG("%s", Name.c_str());
			return std::move(m_Transform);
		}

		for (auto& child : children)
		{
			std::unique_ptr<Transform> t = std::unique_ptr<Transform>(child->getTransform());
			std::unique_ptr<Transform> tran = std::move(t);
			return tran;
		}
	}

	return nullptr;
	

}

//GameObject* GameObject::Search(std::string name) {
//	for (auto& child : children) {
//		if (child->SearchChild(name)) {
//			GameObject* returnChild = child;
//			return child;
//		}
//		return child->Search(name);
//	}
//}
//
//bool GameObject::SearchChild(std::string name) {
//	return name == Name;
//}