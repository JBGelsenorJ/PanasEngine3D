#include "Globals.h"
#include "Module.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "ImGui/imgui.h"
#include "ModuleGUI.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();
	
	Cube cube(1.0f, 1.0f, 1.0f);
	if (App->gui->wireframe == true){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (App->gui->wireframe == false) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (App->gui->cube) {
		App->renderer3D->CreateCubeDirect();
	}

	if (App->gui->pyramid)
	{
		App->renderer3D->CreatePyramid();
	}

	if (App->gui->cylinder)
	{
		App->renderer3D->CreateCylinder(1, 2, 4);
	}

	if (App->gui->sphere)
	{
		App->renderer3D->CreateSphere(1, 24, 48);
	}

	return UPDATE_CONTINUE;
}

