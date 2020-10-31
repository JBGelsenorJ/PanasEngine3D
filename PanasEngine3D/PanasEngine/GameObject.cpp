#pragma once
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ImGui/imgui.h"

#include <vector>

GameObject::GameObject() : active(true), name("Game Object"), parent(nullptr)
{
	CreateComponent(ComponentType::Transform);
}

GameObject::~GameObject()
{
	parent = nullptr;
	components.clear();
	children.clear();
}

void GameObject::Update() {

	for (size_t i = 0; i < components.size(); i++)
	{
		components[i]->Update();
	}

	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->Update();
	}
}

Component* GameObject::GetComponent(ComponentType component) {
	
	for (size_t i = 0; i < components.size(); i++)
	{
		if (components[i]->ReturnType() == component)
		{
			return components[i];
		}
	}

	return nullptr;
}

Component* GameObject::CreateComponent(ComponentType type) {

	Component* component = nullptr;

	switch (type)
	{
		case ComponentType::Transform:
			component = new ComponentTransform();
			components.push_back(component);
			break;
		case ComponentType::Mesh:
			component = new ComponentMesh();
			components.push_back(component);
			break;
		case ComponentType::Material:
			component = new ComponentMaterial();
			components.push_back(component);
			break;
	}

	return component;
}

bool GameObject::DeleteComponent(Component* component)
{
	for (size_t i = 0; i < components.size(); i++)
	{
		if (components[i] == component) {
			delete components[i];
			components.erase(components.begin() + i);
			component = nullptr;
			return true;
		}
	}

	return false;
}
