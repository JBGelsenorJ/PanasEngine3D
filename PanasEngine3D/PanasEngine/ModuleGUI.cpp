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

	brightness = 1.0f;
	width = 1280;
	height = 1024;
	fps = 0;
	fullscreen = false;
	resizable = false;
	borderless = true;
	fulldesktop = false;

	fps_log = { 100, 0 };
	ms_log = { 100, 0 };

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

			GLint major, minor;
			glGetIntegerv(GL_MAJOR_VERSION, &major);
			glGetIntegerv(GL_MINOR_VERSION, &minor);

			ImGui::Text("PANAS ENGINE");

			ImGui::Separator();

			ImGui::TextWrapped("Panas Engine is an engine done by two students of the CITM University for the Engine subject. ");

			ImGui::Separator();

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
			ImGui::BulletText("OpenGL %d.%d", major, minor);
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
			char AppName[32] = "Panas Engine";
			ImGui::InputText("App Name", AppName, IM_ARRAYSIZE(AppName));
			
			char Organization[32] = "CITM";
			ImGui::InputText("Organization", Organization, IM_ARRAYSIZE(Organization));

			ImGui::SliderInt("Max FPS", &fps, 10, 120);
			ImVec4 color(1.0f, 1.0f, 0.0f, 1.0f);
			ImGui::Text("Limit Framerate: ");
			ImGui::SameLine();
			ImGui::TextColored(color, "%i", fps);
		
			char title[25];
			
			sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
			ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

			sprintf_s(title, 25, "Milliseconds %.1f", ms_log[ms_log.size() - 1]);
			ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
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
			ImVec4 color(1.0f, 1.0f, 0.0f, 1.0f);

			ImGui::Text("SDL version: %d.%d.%d", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);
			ImGui::Separator();
			
			int cpus, cache, ram;
			ImGui::Text("CPUs:");
			ImGui::SameLine();
			ImGui::TextColored(color, "%i", cpus = App->GetCPUCount());
			ImGui::SameLine();
			ImGui::TextColored(color, "(Cache: %iKb)", cache = App->GetCPUCache());
			ImGui::Text("RAM: ");
			ImGui::SameLine();
			ImGui::TextColored(color, "%i.0GB", ram = App->GetSystemRAM());
			ImGui::Text("Caps: ");
			ImGui::SameLine();
			ImGui::TextColored(color, "%s", App->GetSystemCaps());
			ImGui::Separator();

			ImGui::Text("Brand:");
			ImGui::SameLine();
			ImGui::TextColored(color, "%s", App->GetBrand());
			ImGui::Text("Model:");
			ImGui::SameLine();
			ImGui::TextColored(color, "%s", App->GetModel());
			ImGui::Text("VRAM Budget:");
			ImGui::SameLine();
			ImGui::TextColored(color, "%i Mb", App->GetBudget());
			ImGui::Text("VRAM Usage:");
			ImGui::SameLine();
			ImGui::TextColored(color, "%i Mb", App->GetUsage());
			ImGui::Text("VRAM Available:");
			ImGui::SameLine();
			ImGui::TextColored(color, "%i Mb", App->GetAvailable());
			ImGui::Text("VRAM Reserved:");
			ImGui::SameLine();
			ImGui::TextColored(color, "%i Mb", App->GetReserved());
		}
		ImGui::End();
	}

	if (viewconsole) {
		ImGui::Begin("Console", &viewconsole);
		
		
	

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

