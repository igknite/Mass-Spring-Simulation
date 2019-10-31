

#pragma once

#include <iostream>
#include <vector>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "cloth.h"
 

using namespace std;


class Simulator
{
public:
	Simulator(void);
	~Simulator(void);

public:
	void					Initialize(void);
 	void					Update();
	void					Render();
	void					Lighting(void);
	void					DrawGround(void);
	void					DrawSphere();

public:
	mass_cloth			*cloth;
	vec3				ground;
	vec3				sphere;
	float				radius;
	bool				m_Sphere;
	float timsStep;
};

