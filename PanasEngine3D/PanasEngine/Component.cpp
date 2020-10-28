#pragma once
#include "Component.h"

Component::Component()
{
	active = true;
	owner = nullptr;
	type = type;
}

Component::~Component()
{

}

void Component::Enable() {

	active = true;

}

void Component::Update() {

}

void Component::Disable() {

	active = false;
}

ComponentType Component::ReturnType() {

	return type;

}

GameObject* Component::ReturnGameObject() {

	return owner;

}
