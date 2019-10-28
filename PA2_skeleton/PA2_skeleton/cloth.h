
#pragma once

#include "spring.h"
#include "Node.h"
#include <stdio.h>
#include <math.h>
#include <vector>
#include <algorithm>

class mass_cloth
{
public:

	std::vector<Node *> nodes;
	std::vector<mass_spring *> spring;
	std::vector<Node*> faces;

	int			size_x, size_y, size_z;
	double		dx, dy, dz;
	double		structural_coef;
	double		shear_coef;
	double		bending_coef;
	int			iteration_n;
	int			drawMode;
	

	mass_cloth()
	{ 	 
	}
	~mass_cloth()
	{ 
		for (int i = 0; i < nodes.size(); i++){ delete nodes[i]; }
		for (int i = 0; i < spring.size(); i++){ delete spring[i]; }
		nodes.clear();
		spring.clear();
		faces.clear();
	}
	enum DrawModeEnum{
		DRAW_MASS_NODES,
		DRAW_SPRINGS,
		DRAW_FACES
	};
 
public:
	void init()
	{
		// Node
		for (int x = size_x; x > 0; x--) {
			for (int y = size_y; y > 0; y--) {
				int z = size_z;
				Node *xp = new Node(vec3(x, z, y)); // change y,z for error detecting
				nodes.push_back(xp);
			}
		}

		//Structural spring row / column
		for (int x = 0; x < size_x; x++) {
			for (int y = 0; y < size_y-1; y++) {
				int z = size_z;
				mass_spring *sp = new mass_spring(nodes[50*x+y], nodes[50*x+y+1]);
				sp->spring_coef = structural_coef;
				spring.push_back(sp);
			}
		}
		for (int x = 0; x < size_x - 1; x++) {
			for (int y = 0; y < size_y; y++) {
				int z = size_z;
				mass_spring *sp = new mass_spring(nodes[50 * x + y], nodes[50 * (x + 1) + y]);
				sp->spring_coef = structural_coef;
				spring.push_back(sp);
			}
		}

		//Shear spring
		for (int x = 0; x < size_x - 1; x++) {
			for (int y = 0; y < size_y - 1; y++) {
				int z = size_z;
				mass_spring *sp = new mass_spring(nodes[50 * x + y], nodes[50 * (x + 1) + y + 1]);
				sp->spring_coef = shear_coef;
				spring.push_back(sp);
			}
		}
		for (int x = 0; x < size_x - 1; x++) {
			for (int y = 1; y < size_y; y++) {
				int z = size_z;
				mass_spring *sp = new mass_spring(nodes[50 * x + y], nodes[50 * (x + 1) + y - 1]);
				sp->spring_coef = shear_coef;
				spring.push_back(sp);
			}
		}

		//bending spring row / column
		for (int x = 0; x < size_x; x++) {
			for (int y = 0; y < size_y - 2; y++) {
				int z = size_z;
				mass_spring *sp = new mass_spring(nodes[50 * x + y], nodes[50 * x + y + 2]);
				sp->spring_coef = bending_coef;
				spring.push_back(sp);
			}
		}
		for (int x = 0; x < size_x - 2; x++) {
			for (int y = 0; y < size_y; y++) {
				int z = size_z;
				mass_spring *sp = new mass_spring(nodes[50 * x + y], nodes[50 * (x + 2) + y]);
				sp->spring_coef = bending_coef;
				spring.push_back(sp);
			}
		}

		//fix point
		nodes[0]->isFixed = true;
		nodes[49]->isFixed = true;

		//face
		for (int x = 0; x < size_x-1; x++) {
			for (int y = 0; y < size_y - 1; y++) {
				faces.push_back(nodes[50 * x + y]);
				faces.push_back(nodes[50 * (x + 1) + y + 1]);
				faces.push_back(nodes[50 * (x + 1) + y]);
			}
		}
		for (int x = 0; x < size_x - 1; x++) {
			for (int y = 0; y < size_y - 1; y++) {
				faces.push_back(nodes[50 * x + y]);
				faces.push_back(nodes[50 * x + y + 1]);
				faces.push_back(nodes[50 * (x + 1) + y + 1]);
			}
		}


		//Basic Implements 1. Init Nodes and Shear and Structural Springs
		//Additional Implements 1. Init Bending Spring
		/*
			Node *xp = new Node(vec3(x, y, z));
		
			mass_spring *sp = new mass_spring(p[Node_Index_A], p[Node_Index_B]);
 			sp->spring_coef = spring_Type_coef;
 			spring.push_back(sp);
		*/
		//Basic Implements 3-1. Generate Faces
		/*
			faces.push_back(p[Node_Index_A]);
			faces.push_back(p[Node_Index_C]);
			faces.push_back(p[Node_Index_B]);
		*/ //done

		//Additional Implements 4-2. Initialize Texture Coordinates	
	}
	
	void computeNormal()
	{
		//Basic Implements 3-2. Compute Vertex Normal
		/*
			for(each face)
			{
				compute face normal
			}
			for(each node)
			{
				인접한 face의 평균 normal
			}
		*/
	}
	
	void add_force(vec3 additional_force)
	{		 
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->add_force(additional_force);
		}
	}

	void compute_force(double dt, vec3 gravity)
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->add_force(gravity * nodes[i]->mass);
		}
		/* Compute Force for all springs */
		for (int i = 0; i < spring.size(); i++)
		{
			spring[i]->internal_force(dt);
		}
	}

	void integrate(double dt)
	{
		/* integrate Nodes*/
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->integrate(dt);
		}
	}
	
	void collision_response(vec3 ground)
	{
		//Basic Implements 4. Collision Check with ground
		//Additional Implements 2. Collision Check with Sphere
		//Additional Implements 3. Collision Check with Mesh Object
		/*
			if(Collision Detection)
			{
				Collision Response
			}
		*/

	}

	void draw();
};