#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "glmath.h"
#include "Light.h"
#include "importer.h"

#include "GLglew/glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")      /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib")    /* link Microsoft OpenGL lib   */
#pragma comment (lib, "GLglew/glew/libx86/glew32.lib")		   /*link glew lib*/

//----------------------------------- cube ----------------------------------------
static const float g_vertex_buffer_data[] = {
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f };

GLfloat vertices[] = { 1, 1, 1,  -1, 1, 1,  -1,-1, 1,   1,-1, 1,   // v0,v1,v2,v3 (front face)
						1, 1, 1,   1,-1, 1,   1,-1,-1,   1, 1,-1,   // v0,v3,v4,v5 (right face)
						1, 1, 1,   1, 1,-1,  -1, 1,-1,  -1, 1, 1,   // v0,v5,v6,v1 (top face)
					   -1, 1, 1,  -1, 1,-1,  -1,-1,-1,  -1,-1, 1,   // v1,v6,v7,v2 (left face)
					   -1,-1,-1,   1,-1,-1,   1,-1, 1,  -1,-1, 1,   // v7,v4,v3,v2 (bottom face)
						1,-1,-1,  -1,-1,-1,  -1, 1,-1,   1, 1,-1 }; // v4,v7,v6,v5 (back face)

uint indices[] = { 0, 1, 2,   2, 3, 0,      // front face
				   4, 5, 6,   6, 7, 4,      // right face
				   8, 9,10,  10,11, 8,      // top face
				   12,13,14,  14,15,12,      // left face
				   16,17,18,  18,19,16,      // bottom face
				   20,21,22,  22,23,20 };    // back face

//----------------------------------- pyramid ----------------------------------------
float pyramid_vertices[] = {
	//Bottom face
	0.0f ,0.0f, 0.0f,
	1.0f ,0.0f, 0.0f,
	1.0f ,0.0f, 1.0f,
	0.0f ,0.0f, 1.0f,

	//Top face
	0.5f, 1.0f, 0.5f
};

uint pyramid_indices[]{
	//Bottom face
	0, 1, 2,	2, 3, 0,

	3, 2, 4, // Front face
	0, 3, 4, // Left face
	2, 1, 4, // Right face
	1, 0, 4  // Back face
};

std::vector<GLfloat> sphere_vertices;
std::vector<GLfloat> sphere_normals;
std::vector<GLfloat> sphere_texcoords;
std::vector<GLushort> sphere_indices;

std::vector<GLfloat> cylinder_vertices;
std::vector<GLfloat> cylinder_normals;
std::vector<GLfloat> cylinder_texcoords;
std::vector<GLushort> cylinder_indices;

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	context = nullptr;
	rendered = false;
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if (context != NULL)
	{
		LOG("OpenGL context successfully created")
	}
	if (context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	GLenum error = glewInit();
	if (error == GL_NO_ERROR)
	{
		LOG("Successfully initializated glew library");
	}
	if (error != GL_NO_ERROR)
	{
		LOG("Error initializing glew library! %s", SDL_GetError());
		ret = false;
	}

	if (ret == true)
	{
		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if (error == GL_NO_ERROR)
		{
			LOG("Successfully initializated OpenGL library")
		}
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);

		mesh = &App->imp->myMesh;

		glGenBuffers(1, (GLuint*)&mesh->id_vertex);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertex * 3, mesh->vertex, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*)&mesh->id_index);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_index, mesh->index, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*)&mesh->id_normals);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * mesh->num_normals * 3, mesh->normals, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*)&mesh->id_texcoords);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_texcoords);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_texcoords * 2, mesh->texcoords, GL_STATIC_DRAW);

		LoadingTextures();

	}

	// Projection matrix for
	OnResize(App->gui->width, App->gui->height);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	Color c = App->camera->background;
	glClearColor(c.r, c.g, c.b, c.a);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{	
	RenderFBX();
	
	if (App->gui->vertexlines)
	{
		DrawVertexNormalLines();
	}
	if (App->gui->facelines)
	{
		DrawFaceNormalLines();
	}
	App->gui->Draw();
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}


void ModuleRenderer3D::LoadFBXBuffer() {
	
	mesh = &App->imp->myMesh;

	glGenBuffers(1, (GLuint*)&mesh->id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertex * 3, mesh->vertex, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&mesh->id_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_index, mesh->index, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&mesh->id_normals);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * mesh->num_normals * 3, mesh->normals, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&mesh->id_texcoords);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_texcoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_texcoords*2, mesh->texcoords, GL_STATIC_DRAW);

}

void ModuleRenderer3D::RenderFBX() {
	if (!App->gui->checker)
	{
		if (!rendered)
		{
			App->imp->LoadTexture("BakerHouseIMG.png");
			rendered = true;
			
		}
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mesh->image_id);
		glBindTexture(GL_TEXTURE_2D, App->imp->Gl_Tex);
	}
	else if (App->gui->checker)
	{
		rendered = false;

		glEnable(GL_TEXTURE_2D);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64,
			0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
		
	}
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
	glNormalPointer(GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_texcoords);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glDrawElements(GL_TRIANGLES, mesh->num_index, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_NORMAL_ARRAY, 0);
	
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}


void ModuleRenderer3D::DrawVertexNormalLines()
{

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);

	for (size_t i = 0; i < App->imp->myMesh.num_vertex * 3; i += 3)
	{
		float v_x = App->imp->myMesh.vertex[i];
		float v_y = App->imp->myMesh.vertex[i + 1];
		float v_z = App->imp->myMesh.vertex[i + 2];

		float n_x = App->imp->myMesh.normals[i];
		float n_y = App->imp->myMesh.normals[i + 1];
		float n_z = App->imp->myMesh.normals[i + 2];

		glVertex3f(v_x, v_y, v_z);
		glVertex3f(v_x + n_x, v_y + n_y, v_z + n_z);
	}

	glEnd();
}

void ModuleRenderer3D::DrawFaceNormalLines() {

	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 0.0f);

	for (size_t i = 0; i < App->imp->myMesh.num_vertex * 3; i += 3)
	{
		float x = (App->imp->myMesh.vertex[i] + App->imp->myMesh.vertex[i + 3] + App->imp->myMesh.vertex[i + 6]) / 3;
		float y = (App->imp->myMesh.vertex[i + 1] + App->imp->myMesh.vertex[i + 4] + App->imp->myMesh.vertex[i + 7]) / 3;
		float z = (App->imp->myMesh.vertex[i + 2] + App->imp->myMesh.vertex[i + 5] + App->imp->myMesh.vertex[i + 8]) / 3;

		float nx = App->imp->myMesh.normals[i];
		float ny = App->imp->myMesh.normals[i + 1];
		float nz = App->imp->myMesh.normals[i + 2];

		glVertex3f(x, y, z);
		glVertex3f(x + nx, y + ny, z + nz);
	}
	glEnd();

}

void ModuleRenderer3D::LoadingTextures() {

	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);

}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::SetDepthtest(bool state) {
	if (state == false)
		glEnable(GL_DEPTH_TEST);
	else if (state == true)
		glDisable(GL_DEPTH_TEST);
}
void ModuleRenderer3D::SetCullface(bool state) {
	if (state == false)
		glEnable(GL_CULL_FACE);
	else if (state == true)
		glDisable(GL_CULL_FACE);
}
void ModuleRenderer3D::SetLighting(bool state) {
	if (state == false)
		glEnable(GL_LIGHTING);
	else if (state == true)
		glDisable(GL_LIGHTING);
}
void ModuleRenderer3D::SetColormaterial(bool state) {
	if (state == false)
		glEnable(GL_COLOR_MATERIAL);
	else if (state == true)
		glDisable(GL_COLOR_MATERIAL);
}
void ModuleRenderer3D::SetTexture2D(bool state) {
	if (state == false)
		glEnable(GL_TEXTURE_2D);
	else if (state == true)
		glDisable(GL_TEXTURE_2D);
}
void ModuleRenderer3D::SetCubemap(bool state) {
	if (state == false)
		glEnable(GL_TEXTURE_CUBE_MAP);
	else if (state == true)
		glDisable(GL_TEXTURE_CUBE_MAP);
}
void ModuleRenderer3D::SetPolygonssmooth(bool state) {
	if (state == false)
		glEnable(GL_POLYGON_SMOOTH);
	else if (state == true)
		glDisable(GL_POLYGON_SMOOTH);
}
void ModuleRenderer3D::CreateCubeDirect() {

	//-------------------------------------Cube made by triangles----------------------------------------------//
	glLineWidth(2.0f);
	glDisable(GL_CULL_FACE);
	glBegin(GL_TRIANGLES);

	glVertex3f(1.f, 1.f, 1.f);
	glVertex3f(-1.f, 1.f, 1.f);
	glVertex3f(-1.f, -1.f, 1.f);

	glVertex3f(-1.f, -1.f, 1.f);
	glVertex3f(1.f, -1.f, 1.f);
	glVertex3f(1.f, 1.f, 1.f);

	glVertex3f(1.f, 1.f, 1.f);
	glVertex3f(1.f, -1.f, 1.f);
	glVertex3f(1.f, -1.f, -1.f);

	glVertex3f(1.f, -1.f, -1.f);
	glVertex3f(1.f, 1.f, -1.f);
	glVertex3f(1.f, 1.f, 1.f);

	glVertex3f(1.f, 1.f, 1.f);
	glVertex3f(1.f, 1.f, -1.f);
	glVertex3f(-1.f, 1.f, -1.f);

	glVertex3f(-1.f, 1.f, -1.f);
	glVertex3f(-1.f, 1.f, 1.f);
	glVertex3f(1.f, 1.f, 1.f);

	glVertex3f(-1.f, 1.f, 1.f);
	glVertex3f(-1.f, 1.f, -1.f);
	glVertex3f(-1.f, -1.f, -1.f);

	glVertex3f(-1.f, -1.f, -1.f);
	glVertex3f(-1.f, -1.f, 1.f);
	glVertex3f(-1.f, 1.f, 1.f);

	glVertex3f(-1.f, -1.f, -1.f);
	glVertex3f(1.f, -1.f, -1.f);
	glVertex3f(1.f, -1.f, 1.f);

	glVertex3f(1.f, -1.f, 1.f);
	glVertex3f(-1.f, -1.f, 1.f);
	glVertex3f(-1.f, -1.f, -1.f);

	glVertex3f(1.f, -1.f, -1.f);
	glVertex3f(-1.f, -1.f, -1.f);
	glVertex3f(-1.f, 1.f, -1.f);

	glVertex3f(-1.f, 1.f, -1.f);
	glVertex3f(1.f, 1.f, -1.f);
	glVertex3f(1.f, -1.f, -1.f);
	glEnd();
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ModuleRenderer3D::CreateCubeVertex() {

	uint my_id = 0;
	glGenBuffers(1, (GLuint*) & (my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data) * 3, g_vertex_buffer_data, GL_STATIC_DRAW);


	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	//… bind and use other buffers
	glDrawArrays(GL_TRIANGLES, 0, sizeof(g_vertex_buffer_data));
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ModuleRenderer3D::CreateCubeIndex() {

	uint my_indices = 0;
	glGenBuffers(1, (GLuint*) & (my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 36, indices, GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ModuleRenderer3D::CreatePyramid() {

	uint my_indices = 0;
	glGenBuffers(1, (GLuint*) & (my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 18, pyramid_indices, GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, pyramid_vertices);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ModuleRenderer3D::CreateSphere(float radius, unsigned int rings, unsigned int sectors) {

	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	int r, s;

	sphere_vertices.resize(rings * sectors * 3);
	sphere_normals.resize(rings * sectors * 3);
	sphere_texcoords.resize(rings * sectors * 2);
	std::vector<GLfloat>::iterator v = sphere_vertices.begin();
	std::vector<GLfloat>::iterator n = sphere_normals.begin();
	std::vector<GLfloat>::iterator t = sphere_texcoords.begin();
	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
		float const y = sin(-M_PI_2 + M_PI * r * R);
		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
		float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

		*t++ = s * S;
		*t++ = r * R;

		*v++ = x * radius;
		*v++ = y * radius;
		*v++ = z * radius;

		*n++ = x;
		*n++ = y;
		*n++ = z;
	}

	sphere_indices.resize(rings * sectors * 4);
	std::vector<GLushort>::iterator i = sphere_indices.begin();
	for (r = 0; r < rings - 1; r++) for (s = 0; s < sectors - 1; s++) {
		*i++ = (r + 1) * sectors + s;
		*i++ = (r + 1) * sectors + (s + 1);
		*i++ = r * sectors + (s + 1);
		*i++ = r * sectors + s;
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &sphere_vertices[0]);
	glNormalPointer(GL_FLOAT, 0, &sphere_normals[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &sphere_texcoords[0]);
	glDrawElements(GL_QUADS, sphere_indices.size(), GL_UNSIGNED_SHORT, &sphere_indices[0]);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ModuleRenderer3D::CreateCylinder(float radius, float height, int sides) {
	const float PI = 3.1415926f;
	int sectorCount = 36;
	float sectorStep = 2 * PI / sectorCount;
	float sectorAngle;  // radian

	std::vector<float> unitCircleVertices;
	for (int i = 0; i <= sectorCount; ++i)
	{
		sectorAngle = i * sectorStep;
		unitCircleVertices.push_back(cos(sectorAngle)); // x
		unitCircleVertices.push_back(sin(sectorAngle)); // y
		unitCircleVertices.push_back(0);                // z
	}

	std::vector<float>().swap(cylinder_vertices);
	std::vector<float>().swap(cylinder_normals);
	std::vector<float>().swap(cylinder_texcoords);

	// get unit circle vectors on XY-plane
	std::vector<float> unitVertices = unitCircleVertices;

	// put side vertices to arrays
	for (int i = 0; i < 2; ++i)
	{
		float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
		float t = 1.0f - i;                              // vertical tex coord; 1 to 0

		for (int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
		{
			float ux = unitVertices[k];
			float uy = unitVertices[k + 1];
			float uz = unitVertices[k + 2];
			// position vector
			cylinder_vertices.push_back(ux * radius);             // vx
			cylinder_vertices.push_back(uy * radius);             // vy
			cylinder_vertices.push_back(h);                       // vz
			// normal vector
			cylinder_normals.push_back(ux);                       // nx
			cylinder_normals.push_back(uy);                       // ny
			cylinder_normals.push_back(uz);                       // nz
			// texture coordinate
			cylinder_texcoords.push_back((float)j / sectorCount); // s
			cylinder_texcoords.push_back(t);                      // t
		}
	}

	// the starting index for the base/top surface
	//NOTE: it is used for generating indices later
	int baseCenterIndex = (int)cylinder_vertices.size() / 3;
	int topCenterIndex = baseCenterIndex + sectorCount + 1; // include center vertex

	// put base and top vertices to arrays
	for (int i = 0; i < 2; ++i)
	{
		float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
		float nz = -1 + i * 2;                           // z value of normal; -1 to 1

		// center point
		cylinder_vertices.push_back(0);     cylinder_vertices.push_back(0);     cylinder_vertices.push_back(h);
		cylinder_normals.push_back(0);      cylinder_normals.push_back(0);      cylinder_normals.push_back(nz);
		cylinder_texcoords.push_back(0.5f); cylinder_texcoords.push_back(0.5f);

		for (int j = 0, k = 0; j < sectorCount; ++j, k += 3)
		{
			float ux = unitVertices[k];
			float uy = unitVertices[k + 1];
			// position vector
			cylinder_vertices.push_back(ux * radius);             // vx
			cylinder_vertices.push_back(uy * radius);             // vy
			cylinder_vertices.push_back(h);                       // vz
			// normal vector
			cylinder_normals.push_back(0);                        // nx
			cylinder_normals.push_back(0);                        // ny
			cylinder_normals.push_back(nz);                       // nz
			// texture coordinate
			cylinder_texcoords.push_back(-ux * 0.5f + 0.5f);      // s
			cylinder_texcoords.push_back(-uy * 0.5f + 0.5f);      // t
		}
	}

	std::vector<int> indices;
	int k1 = 0;                         // 1st vertex index at base
	int k2 = sectorCount + 1;           // 1st vertex index at top

	// indices for the side surface
	for (int i = 0; i < sectorCount; ++i, ++k1, ++k2)
	{
		// 2 triangles per sector
		// k1 => k1+1 => k2
		cylinder_indices.push_back(k1);
		cylinder_indices.push_back(k1 + 1);
		cylinder_indices.push_back(k2);

		// k2 => k1+1 => k2+1
		cylinder_indices.push_back(k2);
		cylinder_indices.push_back(k1 + 1);
		cylinder_indices.push_back(k2 + 1);
	}

	// indices for the base surface
	//NOTE: baseCenterIndex and topCenterIndices are pre-computed during vertex generation
	//      please see the previous code snippet
	for (int i = 0, k = baseCenterIndex + 1; i < sectorCount; ++i, ++k)
	{
		if (i < sectorCount - 1)
		{
			cylinder_indices.push_back(baseCenterIndex);
			cylinder_indices.push_back(k + 1);
			cylinder_indices.push_back(k);
		}
		else // last triangle
		{
			cylinder_indices.push_back(baseCenterIndex);
			cylinder_indices.push_back(baseCenterIndex + 1);
			cylinder_indices.push_back(k);
		}
	}

	// indices for the top surface
	for (int i = 0, k = topCenterIndex + 1; i < sectorCount; ++i, ++k)
	{
		if (i < sectorCount - 1)
		{
			cylinder_indices.push_back(topCenterIndex);
			cylinder_indices.push_back(k);
			cylinder_indices.push_back(k + 1);
		}
		else // last triangle
		{
			cylinder_indices.push_back(topCenterIndex);
			cylinder_indices.push_back(k);
			cylinder_indices.push_back(topCenterIndex + 1);
		}
	}

	glRotatef(90, 1, 0, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &cylinder_vertices[0]);
	glNormalPointer(GL_FLOAT, 0, &cylinder_normals[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &cylinder_texcoords[0]);
	glDrawElements(GL_TRIANGLES, cylinder_indices.size(), GL_UNSIGNED_SHORT, &cylinder_indices[0]);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
