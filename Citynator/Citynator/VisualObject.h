#pragma once
#include <GL/glut.h>
#include <chrono>
#include <cmath>
#include "Utils.h"

class World;

class VisualObject
{
protected:
	double x, y, z;
	World * world;
public:
	VisualObject(double x, double y, double z, World * world);
	~VisualObject();
	virtual void draw() = 0;
	virtual void stepLogic(std::chrono::duration<long long, std::milli>) = 0;
	Utils::point3d getLocation();
};

