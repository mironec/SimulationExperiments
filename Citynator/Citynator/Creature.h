#pragma once
#include "VisualObject.h"

class Creature : public VisualObject
{
public:
	Creature(double x, double y, double z, World * world);
	~Creature();
	void draw();
	void stepLogic(std::chrono::duration<long long, std::milli>);
};

