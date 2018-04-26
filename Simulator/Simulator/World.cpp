#include "World.h"



World::World()
{
	randomGen = std::mt19937_64();
	randomGen.seed(567);

	visibleObjects = std::vector<VisualObject*>();
}


World::~World()
{
	for (size_t i = 0; i < visibleObjects.size(); i++) {
		delete visibleObjects[i];
	}
}

std::mt19937_64 * World::getRandomGen()
{
	return &randomGen;
}

void World::addVisualObject(VisualObject * visualObject)
{
	visibleObjects.push_back(visualObject);
}

void World::render()
{
	glPushMatrix();
	for (size_t i = 0; i < visibleObjects.size(); i++) {
		visibleObjects[i]->draw();
	}
	glPopMatrix();
}

void World::stepLogic(std::chrono::duration<long long, std::milli> duration)
{
	for (std::size_t i = 0; i < visibleObjects.size(); i++) {
		visibleObjects[i]->stepLogic(duration);
	}
}
