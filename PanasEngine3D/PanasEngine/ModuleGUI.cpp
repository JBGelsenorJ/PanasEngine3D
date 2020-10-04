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
	aboutwindow = false;
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
	if (aboutwindow == true) {
		if (ImGui::Begin("About", &aboutwindow)) {
			ImGui::Text("PANAS ENGINE");

			ImGui::Separator();

			ImGui::TextWrapped("Panas Engine is an engine done by two students of the CITM University for the Engine subject. ");

			ImGui::Text("Made by:");
			if (ImGui::Button("Javier D. Belda Gonzalez")) {
				ShellExecuteA(NULL, "open", "https://github.com/JBGelsenorJ", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::SameLine();
			if (ImGui::Button("Silvino Medina Cardona")) {
				ShellExecuteA(NULL, "open", "https://github.com/silvino00", NULL, NULL, SW_SHOWNORMAL);
			}
			
			ImGui::Separator();

			ImGui::Text("Libraries used: ");
			
			ImGui::BulletText("SDL %d.%d.%d", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);
			ImGui::BulletText("SDL Mixer %d.%d.%d", SDL_MIXER_MAJOR_VERSION, SDL_MIXER_MINOR_VERSION, SDL_MIXER_PATCHLEVEL);
			ImGui::BulletText("ImGui %s", ImGui::GetVersion());
			ImGui::BulletText("Glew %d.%d.%d", GLEW_VERSION_MAJOR, GLEW_VERSION_MINOR, GLEW_VERSION_MICRO);
			ImGui::BulletText("OpenGL %d.%d", GL_MAJOR_VERSION, GL_MINOR_VERSION);
			ImGui::BulletText("MathGeoLib 1.5");

			ImGui::Separator();

			ImGui::Text("License: ");
			ImGui::Spacing();
			ImGui::TextWrapped("MIT License");
			ImGui::Spacing();
			ImGui::TextWrapped("Copyright(c) 2020 JBGelsenorJ & silvino00");
			ImGui::Spacing();
			ImGui::TextWrapped("Permission is hereby granted, free of charge, to any person obtaining a copy"
				"of this softwareand associated documentation files(the Software), to deal"
				"in the Software without restriction, including without limitation the rights"
				"to use, copy, modify, merge, publish, distribute, sublicense, and /or sell"
				"copies of the Software, and to permit persons to whom the Software is"
				"furnished to do so, subject to the following conditions :");
			ImGui::Spacing();
			ImGui::TextWrapped("The above copyright noticeand this permission notice shall be included in all"
				"copies or substantial portions of the Software.");
			ImGui::Spacing();
			ImGui::TextWrapped("THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR"
				"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,"
				"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE"
				"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER"
				"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,"
				"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE"
				"SOFTWARE.");

			ImGui::End();
		}
	}

	if (ImGui::BeginMainMenuBar()) {

		//Exit Menu

		if (ImGui::MenuItem("Exit", "(Alt+F4)")) App->closewindow = true;

		//Examples Menu
		if (ImGui::MenuItem("Examples")) {

			menuwindow = !menuwindow;

		};
		
		if (ImGui::BeginMenu("Help")) {

			if (ImGui::MenuItem("Gui Demo", NULL)) {
				menuwindow = !menuwindow;
			}
			if (ImGui::MenuItem("Documentation", NULL)) {
				ShellExecuteA(NULL, "open", "https://github.com/JBGelsenorJ/PanasEngine3D/wiki", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("Download latest", NULL)) {
				ShellExecuteA(NULL, "open", "https://github.com/JBGelsenorJ/PanasEngine3D/releases", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("Report a bug", NULL)) {
				ShellExecuteA(NULL, "open", "https://github.com/JBGelsenorJ/PanasEngine3D/issues", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("About", NULL)) {
				aboutwindow = true;
			}

			ImGui::EndMenu();
		}
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

