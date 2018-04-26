#pragma once
#include <random>
#include <chrono>
#include "VisualObject.h"

class World
{
private:
	std::mt19937_64 randomGen;
	std::vector<VisualObject*> visibleObjects;
public:
	World();
	~World();
	std::mt19937_64 * getRandomGen();
	void addVisualObject(VisualObject * visualObject);
	void render();
	void stepLogic(std::chrono::duration<long long, std::milli> duration);
};

