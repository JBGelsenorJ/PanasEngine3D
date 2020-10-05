#pragma once

#include <vector>
#include <string>
#include "GLglew/glew/include/glew.h"

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleGUI.h"


class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModuleGUI* gui;

private:

	Timer	ms_timer;

	float	dt;

	std::vector<Module*> list_modules;

	

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	bool closewindow;
	

	 int GetCPUCount();
	 int GetCPUCache();
	 int GetSystemRAM();
	 const char* GetSystemCaps();
	 const char* GetBrand();
	 const char* GetModel();

	 int GetBudget();
	 int GetUsage();
	 int GetAvailable();
	 int GetReserved();
	 std::string Caps;



private:
	
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

};
extern Application* App;
