#pragma once
#include "Globals.h"
#include "glmath.h"
#include <vector>
#include "Component.h"

class GameObject;

class ComponentMesh : public Component
{
public:

	ComponentMesh();
	~ComponentMesh();

	void CreateCubeDirect();
	void CreateCubeVertex();
	void CreateCubeIndex();
	void CreatePyramid();
	void CreateSphere(float radius, unsigned int rings, unsigned int sectors);
	void CreateCylinder(float radius, float height, int sides);

public:

};
