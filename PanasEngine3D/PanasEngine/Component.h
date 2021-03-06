#pragma once
#include "Globals.h"
#include <vector>

class GameObject;

enum class ComponentType {
	Mesh,
	Transform,
	Material
};

class Component
{
public:

	Component();
	~Component();

	virtual void Enable();
	virtual void Update();
	virtual void Disable();

	ComponentType ReturnType();
	GameObject* ReturnGameObject();

	
public:

	bool active;
	GameObject* owner;
	ComponentType type;


};