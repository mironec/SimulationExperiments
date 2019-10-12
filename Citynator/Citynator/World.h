#pragma once
#include <random>
#include <chrono>
#include <mutex>
#include <shared_mutex>
#include "VisualObject.h"
#include "Utils.h"

class World
{
private:
	std::mt19937_64 randomGen;
	std::vector<VisualObject*> visibleObjects;
	std::vector<VisualObject*> visibleObjectsToAdd;
	std::vector<VisualObject*> visibleObjectsToDelete;
	mutable std::shared_mutex visibibleObjectsMutex;
public:
	World();
	~World();
	std::mt19937_64 * getRandomGen();
	void addVisualObject(VisualObject * visualObject);
	void removeVisualObject(VisualObject * visualObject);
	std::vector<VisualObject*> getObjectsWithinRange(Utils::point3d point, double range);
	void render();
	void stepLogic(std::chrono::duration<long long, std::milli> duration);
};

