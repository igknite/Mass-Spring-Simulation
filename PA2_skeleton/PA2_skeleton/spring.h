
#pragma once
 
#include <iostream>
#include "Node.h"
using namespace std;

class mass_spring{
public:
	double		spring_coef;
	double		damping_coef;
	Node	*p1;
	Node	*p2;
	double		initial_length;
 
public:
 
	mass_spring(Node *p1, Node *p2)
	{
		damping_coef = 5.0;
		this->p1 = p1;
		this->p2 = p2;
		init();
	}

	void init()
	{
		vec3 S_length = (p2->position - p1->position);
		initial_length = S_length.length();
	}

	void internal_force(double dt)
	{	
		vec3 normalizd = (p1->position - p2->position).Normalizevec();
		vec3 force = (normalizd) * (spring_coef*(p1->position.dist(p2->position) - initial_length) + damping_coef * (p1->velocity - p2->velocity).dot(normalizd));
		p1->add_force(-1*force);
		p2->add_force(force);
		//Basic Implements 2-1. Compute Spring Force
		/*add hook_force and damping force*/
		// node1->add_force(force);
		// node2->add_force(force); //done
	}
	void draw();

};