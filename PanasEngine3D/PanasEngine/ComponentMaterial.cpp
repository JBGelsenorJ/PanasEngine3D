#include "Application.h"
#include "Globals.h"
#include "ComponentMaterial.h"
#include "ImGui/imgui.h"
#include "GLglew/glew/include/glew.h"
#include "Importer.h"

#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib") 
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "GLglew/glew/libx86/glew32.lib")

ComponentMaterial::ComponentMaterial() : Component()
{
	type = ComponentType::Material;
	Gl_Tex = 0;
}

ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::Update()
{
	
}

