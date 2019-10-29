#include "Node.h"

#include <GL/glut.h>
#include "cloth.h"
void Node::draw()
{
	glDisable(GL_LIGHTING);
	glColor3f(0.97, 0.95, 0.15);
	glPointSize(2.0);

	glBegin(GL_POINTS);	
	glVertex3f(getPosX(), getPosY(), getPosZ());
	glEnd();
	glEnable(GL_LIGHTING);
}

void mass_spring::draw()
{
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(2.0);

 	glBegin(GL_LINES);
	glVertex3f(p1->position.x, p1->position.y, p1->position.z);
	glVertex3f(p2->position.x, p2->position.y, p2->position.z);
  	glEnd();	 
	glEnable(GL_LIGHTING);

}

void mass_cloth::draw()
{	
	switch (drawMode)
	{
	case DRAW_MASS_NODES:
		glDisable(GL_LIGHTING);
		for (int i = 0; i < nodes.size(); i++)
			nodes[i]->draw();
		glEnable(GL_LIGHTING);
		break;
	case DRAW_SPRINGS:
		glDisable(GL_LIGHTING);
		for (int i = 0; i < spring.size(); i++)
			spring[i]->draw();
		glEnable(GL_LIGHTING);
		break;
	case DRAW_FACES:
		glColor3f(0.5f, 0.5f, 0.5f);
		glDisable(GL_LIGHTING);
		for (int i = 0; i < faces.size(); i = i + 3) {
			glBegin(GL_TRIANGLES);
				glVertex3f(faces[i]->getPosX(),faces[i]->getPosY(),faces[i]->getPosZ());
				glVertex3f(faces[i + 1]->getPosX(), faces[i + 1]->getPosY(), faces[i + 1]->getPosZ());
				glVertex3f(faces[i + 2]->getPosX(), faces[i + 2]->getPosY(), faces[i + 2]->getPosZ());
				glEnd();
		}
		glEnable(GL_LIGHTING);
		//Basic Implements 3-3. Draw Call for Cloth //done
		//Additional Implements 4-3. Texture Coordinate Mapping
		break;
	default:
		break;
	}
	glPopMatrix();
}

