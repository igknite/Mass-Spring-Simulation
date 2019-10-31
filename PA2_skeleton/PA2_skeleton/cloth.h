
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
				Node *xp = new Node(vec3(size_x-x-25, z, size_y-y-25));
				xp->inipos = vec3(size_x - x, z, size_y - y)/49;
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
		GLuint Texture;
		Texture = LoadTexture("skt_t1.bmp");
		//Additional Implements 4-2. Initialize Texture Coordinates	
	}
	
	void computeNormal()
	{
		for (int i = 0; i < nodes.size(); i++) {
			nodes[i]->normal = vec3(0,0,0);
		} 
		for (int i = 0; i < faces.size(); i += 3) {
			vec3 p1 = faces[i + 1]->position - faces[i]->position;
			vec3 p2 = faces[i + 2]->position - faces[i]->position;
			vec3 N = p1.Cross(p2);
			vec3 NN = N.Normalizevec();
			faces[i]->normal += NN;
			faces[i + 1]->normal += NN;
			faces[i + 2]->normal += NN;
		}
		for (int i = 0; i < nodes.size(); i++) {
			nodes[i]->normal.Normalize();
		}
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
	void collision_response_sphere(vec3 Object, double r) {
		for (int i = 0; i < nodes.size(); i++) {
			vec3 nodetosp = (nodes[i]->position - Object).Normalizevec();
			if ((nodes[i]->position - Object).dot(nodetosp) <= r && nodetosp.dot(nodes[i]->velocity) < 0) {
				vec3 VN = nodetosp.dot(nodes[i]->velocity)*nodetosp;
				vec3 VT = nodes[i]->velocity - VN;
				nodes[i]->velocity = VT - VN;
			}
		}
	}

	void collision_response(vec3 ground)
	{
		for (int i = 0; i < nodes.size(); i++) {
			if ((nodes[i]->position - ground).dot(vec3(0, 1, 0)) <= 1 && vec3(0, 1, 0).dot(nodes[i]->velocity) < 0) {
				nodes[i]->velocity.y *= -0.5;
			}
		}
	}
	GLuint LoadTexture(const char * filename)
	{

		GLuint texture;

		int width, height;

		unsigned char * data;

		FILE * file;

		file = fopen(filename, "rb");

		if (file == NULL) return 0;
		width = 900;
		height = 900;
		data = (unsigned char *)malloc(width * height * 3);
		fread(data, 54, 1, file);
		fread(data, width * height * 3, 1, file);
		fclose(file);

		for (int i = 0; i < width * height; ++i)
		{
			int index = i * 3;
			unsigned char B, R;
			B = data[index];
			R = data[index + 2];

			data[index] = R;
			data[index + 2] = B;

		}


		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);


		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
		free(data);

		return texture;
	}
	void draw();
};