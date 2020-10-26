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

	Component* CreateComponent(ComponentType type);
	

public:

	bool active;
	std::string name;
	std::vector<Component*> components;
};

