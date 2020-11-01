#pragma once
#include "Globals.h"
#include "glmath.h"
#include <vector>
#include "Component.h"

class GameObject;
typedef unsigned int GLuint;
typedef unsigned char GLubyte;

class ComponentMaterial : public Component
{

public:

	ComponentMaterial();
	~ComponentMaterial();
	virtual void Update();

public:

	GLuint Gl_Tex;
	

public:
	
};
