#ifndef _MODULEGUI_H_
#define _MODULEGUI_H_

#include "Module.h"
#include "Globals.h"
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

private:

};

#endif // !_MODULEGUI_H_

