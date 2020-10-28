#pragma once
#include "Globals.h"
#include "Module.h"
#include "ModuleRenderer3D.h"

#include "Devil/include/IL/ilu.h"
#include "Devil/include/IL/ilut.h"

#pragma comment( lib, "Devil/libx86/DevIL.lib" )
#pragma comment( lib, "Devil/libx86/ILU.lib" )
#pragma comment( lib, "Devil/libx86/ILUT.lib" )

struct Mesh
{
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;

	uint	id_normals = 0;
	uint	num_normals = 0;
	float*	normals = NULL;

	uint	id_colors = 0;
	uint	num_colors = 0;
	float* colors = NULL;

	uint	id_texcoords = 0;
	uint	num_texcoords = 0;
	float* texcoords = nullptr;
	uint image_id;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;
};

class Importer : public Module
{
public:
	Importer(Application* app, bool start_enabled = true);
	~Importer();

	bool Init();
	bool CleanUp();

	void UploadFile(char* file_path, int id);

	void LoadTexture(char* path);
	char* GetMeshFileName();
	char* GetMaterialFileName();
private:
	char* meshfilename;
	char* materialfilename;
public:

	Mesh myMesh;
	GLuint Gl_Tex;
	
};