#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);
	void SetFullScreen(bool state);
	void SetBorderless(bool state);
	void SetResizable(bool state);
	void SetFullDesktop(bool state);
	void SetBright(float bright);
	void SetWidth(int width);
	void SetHeight(int height);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;


};

#endif // __ModuleWindow_H__