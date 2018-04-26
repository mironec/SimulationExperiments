#include "VisualObject.h"



VisualObject::VisualObject(double x, double y, double z, World * world)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->world = world;
}


VisualObject::~VisualObject()
{
}

Utils::point3d VisualObject::getLocation()
{
	return Utils::point3d{ x, y, z };
}
