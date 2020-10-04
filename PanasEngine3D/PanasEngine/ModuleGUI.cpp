#include "Globals.h"
#include "ModuleGUI.h"
#include "Application.h"

#include "GLglew/glew/include/glew.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "ImGui/imgui_internal.h"
#include "MathGeoLib/include/MathGeoLib.h"


ModuleGUI::ModuleGUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	menuwindow = false;
	brightness = 1.0f;
	width = 1280;
	height = 1024;
	fullscreen = false;
	resizable = false;
	borderless = true;
	fulldesktop = false;
}

ModuleGUI::~ModuleGUI()
{

}

bool ModuleGUI::Init()
{

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsClassic();

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

		if (ImGui::MenuItem("Exit", "(Alt+F4)")) App->closewindow = true;

		//Examples Menu
		if (ImGui::MenuItem("Examples")) {

			menuwindow = !menuwindow;

		};
		if (ImGui::MenuItem("Objects", NULL, true)) {
		
			/*ImGui::MenuItem("Cube", NULL, true);*/
		
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::Checkbox("Console", &viewconsole)) { /* Do stuff */ }
			if (ImGui::MenuItem("Configuration")) {
				viewconfiguration = !viewconfiguration;
			}
			ImGui::EndMenu();
		}
	}

	ImGui::EndMainMenuBar();

	if (menuwindow) {
		ImGui::ShowDemoWindow(&menuwindow);
	}
	if (viewconfiguration)
	{
		ImGui::Begin("Configuration", &viewconfiguration);
		if (ImGui::CollapsingHeader("Application"))
		{
			
		}
		if (ImGui::CollapsingHeader("Window"))
		{
			ImGui::SliderFloat("Brightness", &brightness, 0.f, 1.0f);
			App->window->SetBright(brightness);

			ImGui::SliderInt("Width", &width, 640, 1920);
			App->window->SetWidth(width);

			ImGui::SliderInt("Height", &height, 480, 1080);
			App->window->SetHeight(height);

			
			ImGui::Checkbox("Fullscreen", &fullscreen);
			App->window->SetFullScreen(fullscreen);
			ImGui::SameLine();

			ImGui::Checkbox("Resizable", &resizable);
			App->window->SetResizable(resizable);
			
			ImGui::Checkbox("Borderless", &borderless);
			App->window->SetBorderless(borderless);
			ImGui::SameLine();
			
			ImGui::Checkbox("Fulldesktop", &fulldesktop);
			App->window->SetFullDesktop(fulldesktop);

		}

		if (ImGui::CollapsingHeader("Hardware"))
		{

		}
		ImGui::End();
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

