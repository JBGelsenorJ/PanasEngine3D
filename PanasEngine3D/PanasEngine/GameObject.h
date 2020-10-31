#pragma once
#include "Globals.h"
#include <vector>
#include <iostream>

class Component;
enum class ComponentType;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();

	Component* GetComponent(ComponentType component);
	Component* CreateComponent(ComponentType type);
	bool DeleteComponent(Component* component);

public:

	GameObject* parent;
	bool active;
	char* name;
	std::vector<Component*> components;
	std::vector<GameObject*> children;
};

