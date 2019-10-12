#include "Plant.h"

Plant::Plant(double x, double y, double z, World * world) : VisualObject(x, y, z, world) {
	this->areaSize = 0.1f;
}


Plant::~Plant()
{
}

void Plant::draw()
{
	glPushMatrix();
	glTranslated(x, y, z);
	glColor3f(0.0f, 1.0f, 0.0f);
	GLUquadric * ptr = Utils::circleQuadric;
	gluDisk(ptr, 0.0f, getRadius(), 200, 1);
	glPopMatrix();
}

double Plant::getRadius() {
	return sqrt(areaSize / std::_Pi);
}

void Plant::stepLogic(std::chrono::duration<long long, std::milli> timeDelta)
{
	areaSize += sqrt(areaSize) * timeDelta.count() / 1000.0l / 1000.0l;
	areaSize -= areaSize * timeDelta.count() / 1000.0l / 1000.0l;

	std::mt19937_64 &rand = *(world->getRandomGen());
	if (rand() % (1000 * 1000 * 1000) < (unsigned)timeDelta.count()) {
		double angle = (double)(rand() % 360000);
		angle = angle / 1000.0l;
		angle = angle / 180.0l * std::_Pi;
		double xn, yn, zn;
		xn = x + cos(angle) * getRadius();
		yn = y + sin(angle) * getRadius();
		zn = z;
		Plant * p = new Plant(xn, yn, zn, world);
		world->addVisualObject(p);
	};
}

