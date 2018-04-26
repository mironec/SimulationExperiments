#include "World.h"



World::World()
{
	randomGen = std::mt19937_64();
	randomGen.seed(567);

	visibleObjects = std::vector<VisualObject*>();
	visibleObjectsToAdd = std::vector<VisualObject*>();
	visibleObjectsToDelete = std::vector<VisualObject*>();
}


World::~World()
{
	for (size_t i = 0; i < visibleObjects.size(); i++) {
		delete visibleObjects[i];
	}
	for (size_t i = 0; i < visibleObjectsToAdd.size(); i++) {
		delete visibleObjectsToAdd[i];
	}
}

std::mt19937_64 * World::getRandomGen()
{
	return &randomGen;
}

void World::addVisualObject(VisualObject * visualObject) {
	visibleObjectsToAdd.push_back(visualObject);
}

//TODO: currently O(n), could exchange data types to something more efficient
void World::removeVisualObject(VisualObject * visualObject) {
	visibleObjectsToDelete.push_back(visualObject);
}

std::vector<VisualObject*> World::getObjectsWithinRange(Utils::point3d point, double range)
{
	std::vector<VisualObject*> toReturn;
	for (auto iter = visibleObjects.begin(); iter < visibleObjects.end(); iter++) {
		auto curr = *iter;
		if (Utils::point3dDistance(curr->getLocation(), point) < range) {
			toReturn.push_back(curr);
		}
	}

	return toReturn;
}

//TODO: FIX RACE CONDITION with logic thread, which removes and adds visible objects
void World::render()
{
	glPushMatrix();
	visibleObjectsMutex.lock();
	for (size_t i = 0; i < visibleObjects.size(); i++) {
		visibleObjects[i]->draw();
	}
	visibleObjectsMutex.unlock();
	glPopMatrix();
}

void World::stepLogic(std::chrono::duration<long long, std::milli> duration)
{
	visibleObjectsMutex.lock();
	for (auto iter = visibleObjectsToAdd.begin(); iter != visibleObjectsToAdd.end(); iter++) {
		visibleObjects.push_back(*iter);
	} visibleObjectsToAdd.clear();
	
	auto iter = visibleObjects.begin();
	while( iter != visibleObjects.end() ) {
		bool deleted = false;

		for (auto iterDel = visibleObjectsToDelete.begin(); iterDel != visibleObjectsToDelete.end(); iterDel++) {
			if (*iter == *iterDel) {
				iter = visibleObjects.erase(iter);
				deleted = true;
				break;
			}
		}

		if(!deleted) iter++;
	} visibleObjectsToDelete.clear();

	for (std::size_t i = 0; i < visibleObjects.size(); i++) {
		visibleObjects[i]->stepLogic(duration);
	}
	visibleObjectsMutex.unlock();
}
