#pragma once
#include <GL/glut.h>
#include <cmath>

namespace Utils {
	extern GLUquadric * circleQuadric;
	void strokeString(const char* str);
	enum TASK { NO_ACTION, WALK };
	typedef struct point3d {
		double x, y, z;
	} point3d;
	double point3dDistance(const point3d &a, const point3d &b);
}