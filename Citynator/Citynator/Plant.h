#pragma once
#include "VisualObject.h"
#include "Utils.h"
#include "World.h"
#include <iostream>
#include <random>

class Plant : public VisualObject
{
private:
	double areaSize;
	double getRadius();
public:
	Plant(double x, double y, double z, World * world);
	~Plant();
	void draw();
	void stepLogic(std::chrono::duration<long long, std::milli> timeDelta);
};

