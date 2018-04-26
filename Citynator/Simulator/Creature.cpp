#include "Creature.h"

Creature::Creature(double x, double y, double z, World * world) : VisualObject(x, y, z, world) {}


Creature::~Creature()
{
}

void Creature::draw()
{
	glPushMatrix();
	glTranslated(x, y, z);
	glColor3f(1.0f, 1.0f, 0.0f);
	GLUquadric * ptr = gluNewQuadric();
	gluDisk(ptr, 0.0f, 1.0f, 200, 1);
	glPopMatrix();
}

void Creature::stepLogic(std::chrono::duration<long long, std::milli>)
{
	
}
