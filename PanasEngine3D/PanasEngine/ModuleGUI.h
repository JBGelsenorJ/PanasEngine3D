#ifndef _MODULEGUI_H_
#define _MODULEGUI_H_

#include "Globals.h"
#include "Module.h"
#include <vector>
#include <string>
#include "ImGui/imgui.h"

class ModuleGUI : public Module
{
public:
	ModuleGUI::ModuleGUI(Application* app, bool start_enabled = true);
	ModuleGUI::~ModuleGUI();

	bool Init();
	update_status DockSpace(bool* p_open);
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	void AddLogText(std::string incoming_text);
	void Draw();

	int width;
	int height;
	int fps;

	bool activeScreen;

	bool wireframe;
	bool vertexlines;
	bool facelines;
	bool checker;

	bool cube;
	bool pyramid;
	bool cylinder;
	bool sphere;

private:
	bool menuwindow;
	bool aboutwindow;
	bool viewconsole;
	bool viewconfiguration;
	bool viewhierarchy;
	bool fullscreen;
	bool resizable;
	bool borderless;
	bool fulldesktop;
	
	bool depthtest;
	bool cullface;
	bool lighting;
	bool colormaterial;
	bool texture2D;
	bool cubemap;
	bool polygonssmooth;
	
	float brightness;

	bool* dockingwindow;

	std::vector<float> fps_log;
	std::vector<float> ms_log;

	std::vector<std::string> log_record;

};

#endif // !_MODULEGUI_H_

