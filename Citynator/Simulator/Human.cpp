#include "Human.h"
#include "World.h"
#include "Plant.h"
#include <GL/glut.h>
#include <sstream>
#include "Utils.h"

Human::Human(double x, double y, double z, World * world) : Creature(x, y, z, world) {
	saturation = 0.5;
	currentTaskID = Utils::TASK::NO_ACTION;
	currentTaskObject = nullptr;
	movementSpeed = defaultMovementSpeed;
}


Human::~Human() {}

void Human::stepLogic(std::chrono::duration<long long, std::milli> timeDelta) {
	saturation -= timeDelta.count() * saturationDrainByIdling;

	//Hungry - seek out food
	if (saturation <= 0.9 && currentTaskID == Utils::TASK::NO_ACTION) {
		currentTaskID = Utils::TASK::WALK;

		//Look for nearby food
		std::vector<VisualObject*> locale = world->getObjectsWithinRange(getLocation(), sightRange);

		double minimumDist = sightRange * 2;
		Plant* toEat = nullptr;

		for (auto iter = locale.begin(); iter < locale.end(); iter++) {
			auto curr = *iter;
			Plant* plant;
			if ( (plant = dynamic_cast<Plant*>(curr)) != nullptr) {
				double currentDist = Utils::point3dDistance(plant->getLocation(), getLocation());
				if (currentDist < minimumDist) {
					minimumDist = currentDist;
					toEat = plant;
				}
			}
		}

		if (toEat != nullptr) {
			if (minimumDist < 0.001) {
				currentTaskID = Utils::TASK::NO_ACTION;
				world->removeVisualObject(toEat);
				saturation += 0.1;
			}
			else currentTaskObject = new Utils::point3d(toEat->getLocation());
		}
		else {
			//Move randomly in hopes of finding food
			auto r = *world->getRandomGen();
			double rx = x + ((r() % 1000000l) / 1000000.0 - 0.5) * sightRange;
			double ry = y + ((r() % 1000000l) / 1000000.0 - 0.5) * sightRange;
			double rz = z + ((r() % 1000000l) / 1000000.0 - 0.5) * sightRange;
			currentTaskObject = new Utils::point3d{ rx, ry, rz };
		}
	}

	//Birth of a new human --- TODO: Mating
	if (saturation >= 0.9 && ((*world->getRandomGen())() % reproductionRate <= (unsigned)timeDelta.count())) {
		saturation -= 0.5;
		Human* baby = new Human(x, y, z, world);
		world->addVisualObject(baby);
	}

	//Walking routine --- TODO: Pathfinding, collisions, obstacles
	if (currentTaskID == Utils::TASK::WALK) {
		double budget = movementSpeed * timeDelta.count() / 1000.0;

		//We can move there this tick - save some energy
		Utils::point3d targetPos = *static_cast<Utils::point3d*>(currentTaskObject);
		double target = Utils::point3dDistance(getLocation(), targetPos);
		if (target <= budget) {
			saturation -= timeDelta.count() * (target / budget) * saturationDrainByWalking;
			x = targetPos.x;
			y = targetPos.y;
			z = targetPos.z;
			currentTaskID = Utils::TASK::NO_ACTION;
			delete currentTaskObject;
			currentTaskObject = nullptr;
		}
		//Cannot get there now, move closer
		else {
			saturation -= timeDelta.count() * saturationDrainByWalking;
			double xd = targetPos.x - x;
			double yd = targetPos.y - y;
			double zd = targetPos.z - z;
			
			x += budget * xd / target;
			y += budget * yd / target;
			z += budget * zd / target;
		}
	}

	if (saturation <= 0.0) this->world->removeVisualObject(this);
}

void Human::draw() {
	Creature::draw();
	glPushMatrix();
	glTranslated(x - 0.5, y + 1.1, z + 1.0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glScalef(0.01f, 0.01f, 0.01f);

	std::stringstream ss; ss << saturation;
	Utils::strokeString(ss.str().c_str());

	glPopMatrix();
}
