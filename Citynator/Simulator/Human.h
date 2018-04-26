#pragma once
#include "Creature.h"
#include "Utils.h"

class Human :
	public Creature
{
private:
	const double saturationDrainByIdling = 1.0 / 5.0 / 86400.0 / 1000.0;	//per one milli
	const double saturationDrainByWalking = 2.0 / 86400.0 / 1000.0;			//per one milli
	const double sightRange = 10.0;											//meters
	const double defaultMovementSpeed = 0.1;								//meters per second
	const long long unsigned int reproductionRate = 1l * 86400l * 1000l;	//chance each milli to spawn a new human

	double saturation;
	double movementSpeed;
	Utils::TASK currentTaskID;
	void* currentTaskObject;
public:
	Human(double x, double y, double z, World * world);
	~Human();
	void stepLogic(std::chrono::duration<long long, std::milli>);
	void draw();
};

