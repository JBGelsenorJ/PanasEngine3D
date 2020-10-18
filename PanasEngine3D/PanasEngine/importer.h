#pragma once
#include "Globals.h"
#include "Module.h"

struct Mesh
{
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;

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

	void UploadFile(char* file_path);
	
public:
	Mesh myMesh;
};