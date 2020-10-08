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

	depthtest = false;
	cullface = false;
	lighting = false;
	colormaterial = false;
	texture2D = false;
	cubemap = false;
	polygonssmooth = false;

	wireframe = false;


	fps_log = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	ms_log = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

	viewconfiguration = false;
	viewconsole = false;
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
			ImVec4 color(1.0f, 1.0f, 0.0f, 1.0f);

			ImGui::BulletText("SDL ");
			ImGui::SameLine();
			ImGui::TextColored( color, "%d.%d.%d", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);
			ImGui::BulletText("SDL Mixer ");
			ImGui::SameLine();
			ImGui::TextColored( color, "%d.%d.%d", SDL_MIXER_MAJOR_VERSION, SDL_MIXER_MINOR_VERSION, SDL_MIXER_PATCHLEVEL);
			ImGui::BulletText("ImGui ");
			ImGui::SameLine();
			ImGui::TextColored(color, "%s", ImGui::GetVersion());
			ImGui::BulletText("Glew ");
			ImGui::SameLine();
			ImGui::TextColored(color, "%d.%d.%d", GLEW_VERSION_MAJOR, GLEW_VERSION_MINOR, GLEW_VERSION_MICRO);
			ImGui::BulletText("OpenGL ");
			ImGui::SameLine();
			ImGui::TextColored(color, "%d.%d", major, minor);
			ImGui::BulletText("MathGeoLib");
			ImGui::SameLine();
			ImGui::TextColored(color, "1.5");

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
		
		if (ImGui::BeginMenu("Draw mode"))
		{
			if (ImGui::MenuItem("Solid")) {
				wireframe = false; 
			}
			if (ImGui::MenuItem("Wireframe")) {
				wireframe = true;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Console")) {
				viewconsole = !viewconsole;
			}
			if (ImGui::MenuItem("Configuration")) {
				viewconfiguration = !viewconfiguration;
			}
			ImGui::EndMenu();
		}

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
	if (viewconfiguration)
	{
		ImGui::Begin("Configuration", &viewconfiguration);
		if (ImGui::CollapsingHeader("Application"))
		{
			char AppName[32] = "Panas Engine";
			ImGui::InputText("App Name", AppName, IM_ARRAYSIZE(AppName));
			
			char Organization[32] = "CITM";
			ImGui::InputText("Organization", Organization, IM_ARRAYSIZE(Organization));
			
			ImVec4 color(1.0f, 1.0f, 0.0f, 1.0f);
			int fps_c = 1000 / App->capped_ms;
			
			if (ImGui::SliderInt("Max FPS", &fps_c, 10, 120)) App->capped_ms = 1000 / fps_c;
			
			ImGui::Text("Limit Framerate: ");
			ImGui::SameLine();
			ImGui::TextColored(color, "%i", fps);
		
			fps_log.erase(fps_log.begin());
			fps_log.push_back(App->fps);
			ms_log.erase(ms_log.begin());
			ms_log.push_back(App->dt * 1000);

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
		if (ImGui::CollapsingHeader("Renderer")) {

			ImGui::Text("OPENGL OPTIONS: ");
			
			ImGui::BulletText("General");
			if (ImGui::Checkbox("Depth Test", &depthtest)) {
				App->renderer3D->SetDepthtest(depthtest);
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Cull Face", &cullface)) {
				App->renderer3D->SetCullface(cullface);
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Lightning", &lighting)) {
				App->renderer3D->SetLighting(lighting);
			}
			
			ImGui::BulletText("Polygons");
			if (ImGui::Checkbox("Polygons smooth", &polygonssmooth)) {
				App->renderer3D->SetPolygonssmooth(polygonssmooth);
			}
			
			ImGui::BulletText("Color");
			if (ImGui::Checkbox("Color Material", &colormaterial)) {
				App->renderer3D->SetColormaterial(colormaterial);
			}

			ImGui::BulletText("Textures");
			if (ImGui::Checkbox("2D", &texture2D)) {
				App->renderer3D->SetTexture2D(texture2D);
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Cube Map", &cubemap)) {
				App->renderer3D->SetCubemap(cubemap);
			}
			
		}
		ImGui::End();
	}
	
	if (viewconsole) {
		ImGui::Begin("Console", &viewconsole);

		for (int i = 0; i < log_record.size(); i++)
		{
			ImGui::Text("%s", log_record[i].c_str());
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

	log_record.clear();
	fps_log.clear();
	ms_log.clear();

	return true;
}

void ModuleGUI::AddLogText(std::string incoming_text) {

	if (&log_record != NULL) {
		log_record.push_back(incoming_text);
	}
	
}

