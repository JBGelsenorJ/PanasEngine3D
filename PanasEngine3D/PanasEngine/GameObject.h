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
	void AddComponent(Component* component);

public:

	bool active;
	std::string name;
	std::vector<Component*> components;
};

