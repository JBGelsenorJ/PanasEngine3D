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
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	

	int width;
	int height;

private:
	bool menuwindow;
	bool viewconsole;
	bool viewconfiguration;
	bool fullscreen;
	bool resizable;
	bool borderless;
	bool fulldesktop;


	float brightness;




};

#endif // !_MODULEGUI_H_

