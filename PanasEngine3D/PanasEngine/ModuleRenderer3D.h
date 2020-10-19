#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "importer.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	void SetDepthtest(bool state);
	void SetCullface(bool state);
	void SetLighting(bool state);
	void SetColormaterial(bool state);
	void SetTexture2D(bool state);
	void SetCubemap(bool state);
	void SetPolygonssmooth(bool state);
	void RenderFBX();
	void LoadFBXBuffer();
		
	void CreateCubeDirect();
	void CreateCubeVertex();
	void CreateCubeIndex();
	void CreatePyramid();
	void CreateSphere(float radius, unsigned int rings, unsigned int sectors);
	void CreateCylinder(float radius, float height, int sides);
public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	Mesh* mesh;
};