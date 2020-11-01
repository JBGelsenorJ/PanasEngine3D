#pragma once
#include "Module.h"
#include "Globals.h"
#include "ImGui/imgui.h"

class GameObject;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	GameObject* CreateGameObject(GameObject* parent);

public:
	std::vector<GameObject*> game_objects;

	GameObject* selected;

	std::string name;

};
