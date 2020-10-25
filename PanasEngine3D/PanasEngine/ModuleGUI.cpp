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
	borderless = false;
	fulldesktop = false;

	depthtest = false;
	cullface = false;
	lighting = false;
	colormaterial = false;
	texture2D = false;
	cubemap = true;
	polygonssmooth = false;

	cube = false;
	pyramid = false;
	cylinder = false;
	sphere = false;

	wireframe = false;
	vertexlines = false;
	facelines = false;
	checker = false;

	fps_log = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	ms_log = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

	viewconfiguration = true;
	viewconsole = true;
	viewhierarchy = true;
	viewinspector = true;
	
	
}

ModuleGUI::~ModuleGUI()
{

}

bool ModuleGUI::Init()
{

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::StyleColorsDark();

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
	DockSpace(dockingwindow);

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
			ImGui::TextColored(color, "%d.%d.%d", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);
			ImGui::BulletText("SDL Mixer ");
			ImGui::SameLine();
			ImGui::TextColored(color, "%d.%d.%d", SDL_MIXER_MAJOR_VERSION, SDL_MIXER_MINOR_VERSION, SDL_MIXER_PATCHLEVEL);
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

		if (ImGui::BeginMenu("File")) {

			if (ImGui::MenuItem("Save", NULL)) {
			}
			if (ImGui::MenuItem("Load", NULL)) {
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Editor Windows", NULL)) {
				viewconfiguration = !viewconfiguration;
				viewconsole = !viewconsole;
				viewhierarchy = !viewhierarchy;
			}

			ImGui::Separator();

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

			ImGui::Separator();

			if (ImGui::MenuItem("Exit", "(Alt+F4)")) App->closewindow = true;


			ImGui::EndMenu();
		}
		
		//Examples Menu
		if (ImGui::MenuItem("Examples")) {

			menuwindow = !menuwindow;

		};

		if (ImGui::BeginMenu("Create GameObject"))
		{
			if (ImGui::MenuItem("Cube")) {
				pyramid = false;
				cylinder = false;
				sphere = false;
				cube = !cube;
				if (cube)
				{
					LOG("Cube primitive created");
				}
			}
			if (ImGui::MenuItem("Pyramid")) {
				cube = false;
				cylinder = false;
				sphere = false;
				pyramid = !pyramid;
				if (pyramid)
				{
					LOG("Pyramid primitive created")
				}	
			}
			if (ImGui::MenuItem("Cylinder")) {
				pyramid = false;
				cube = false;
				sphere = false;
				cylinder = !cylinder;
				if (cylinder)
				{
					LOG("Cylinder primitive created")
				}
			}
			if (ImGui::MenuItem("Sphere")) {
				pyramid = false;
				cylinder = false;
				cube = false;
				sphere = !sphere;
				if (sphere)
				{
					LOG("Sphere primitive created")
				}
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Draw mode"))
		{
			/*if (ImGui::MenuItem("Solid")) {
				wireframe = false;
			}
			if (ImGui::MenuItem("Wireframe")) {
				wireframe = true;
			}
			if (ImGui::MenuItem("VertexLines")) {
				vertexlines = !vertexlines;
			}
			if (ImGui::MenuItem("FaceLines")) {
				facelines = !facelines;
			}*/
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
			if (ImGui::MenuItem("Hierarchy")) {
				viewhierarchy = !viewhierarchy;
			}
			if (ImGui::MenuItem("Inspector"))
			{
				viewinspector = !viewinspector;
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

	if (viewhierarchy)
	{
		ImGui::Begin("Hierarchy", &viewhierarchy);
		ImGui::End();
	}
	if (viewinspector)
	{
		ImGui::Begin("Inspector", &viewhierarchy);
		if (ImGui::CollapsingHeader("Local Transformation"))
		{
			ImGui::Separator();
			vec3 position = {0,0,0};
			vec3 rotation = {0,0,0};
			vec3 scale = {0,0,0};
			if (ImGui::DragFloat3("Position",&position, 0.1f))
			{
				
			}

			if (ImGui::DragFloat3("Rotation", &rotation, 0.1f))
			{
				
			}

			if (ImGui::DragFloat3("Scale", &scale, 0.1f))
			{
				
			}
		}
		if (ImGui::CollapsingHeader("Mesh"))
		{
			ImGui::Separator();
			ImGui::Text("File:");
			ImGui::SameLine();
			ImGui::TextColored({ 1.0f, 1.0f, 0.0f, 1.0f }, "%s", App->imp->GetMeshFileName());
			ImGui::Separator();
			ImGui::BulletText("General");

			if (ImGui::Checkbox("Wireframe", &wireframe));

			if (ImGui::Checkbox("See Vertex Lines (Red)", &vertexlines));

			if (ImGui::Checkbox("See Face Lines (Yellow)", &facelines));

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

		}
		if (ImGui::CollapsingHeader("Material"))
		{
			ImGui::Separator();
			ImGui::Text("File:");
			ImGui::SameLine();
			ImGui::TextColored({ 1.0f, 1.0f, 0.0f, 1.0f }, "%s", App->imp->GetMaterialFileName());
			ImGui::Separator();

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

			if (ImGui::Checkbox("Checker Mode", &checker));			
		}
		ImGui::End();
	}
	
	return UPDATE_CONTINUE;
}

update_status ModuleGUI::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

void ModuleGUI::Draw() {

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

update_status ModuleGUI::DockSpace(bool* p_open)
{
	update_status ret = UPDATE_CONTINUE;

	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", p_open, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	ImGui::End();

	return ret;
}

