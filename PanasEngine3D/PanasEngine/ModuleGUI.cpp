#include "Globals.h"
#include "ModuleGUI.h"
#include "Application.h"

#include "GLglew/glew/include/glew.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "ImGui/imgui_internal.h"
#include <gl/GL.h>


ModuleGUI::ModuleGUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleGUI::~ModuleGUI()
{

}

bool ModuleGUI::Init()
{
	menuwindow = true;
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();



	ImGuiIO& io = ImGui::GetIO(); (void)io;


	// Setup Dear ImGui style
	ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplOpenGL3_Init();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);

	return true;
}

update_status ModuleGUI::PreUpdate(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleGUI::Update(float dt)
{
	//Main menu bar
	if (ImGui::BeginMainMenuBar()) {

		//Exit Menu
		/*if (ImGui::MenuItem("Exit", "(Alt+F4)")) App->closeEngine = true;*/

		//Examples Menu
		if (ImGui::MenuItem("Examples")) {

			menuwindow = !menuwindow;

		};
	}

	ImGui::EndMainMenuBar();


	if (menuwindow) {
		ImGui::ShowDemoWindow(&menuwindow);
	}


	return UPDATE_CONTINUE;
}

update_status ModuleGUI::PostUpdate(float dt)
{
	
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	return UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp() {
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

