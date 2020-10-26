#pragma once
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

void GameObject::Update() {

}

Component* GameObject::CreateComponent(ComponentType type) {
	
	Component* component = nullptr;

	return component;
}
