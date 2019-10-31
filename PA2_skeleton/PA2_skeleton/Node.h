

#pragma once
#include <stdio.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include "vector.h"
class Node
{
public:
	double	mass;
	vec3	force;
	vec3	position;
	vec3	velocity;
	vec3	acceleration;
	vec3	normal;
	bool	isFixed;
	vec3	inipos;

public:
	Node(void)
	{
		isFixed = false;
	}
	Node(vec3 init_pos)
	{
		isFixed = false;
		position = init_pos;
		mass = 1.0;
	}
 
	~Node(void)
	{
	}

	double	getPosX(void) { return position.getX(); }
	double	getPosY(void) { return position.getY(); }
	double	getPosZ(void){ return position.getZ(); }

	void add_force(vec3 additional_force)
	{
		force += additional_force;
	}

	void integrate(double dt)
	{
		if (!isFixed)
		{	
			if (position.y < -14.5) {
				position.y = -14.5;
				velocity.y = 0;
			}
			acceleration = force / mass;
			velocity = acceleration * dt + velocity;
			position = position + velocity * dt;

			//Basic Implements 2-2. Integration 
			// done
		}
		/*initialize Force*/
		force.x = force.y = force.z= 0.0;
	}

	void draw();
};
