#pragma once
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

GameObject::GameObject()
{
	active = true;

}

GameObject::~GameObject()
{
	
}

void GameObject::Update() {

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


void GameObject::AddComponent(Component* component) {

	components.push_back(component);

}
