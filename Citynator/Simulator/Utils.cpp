#include "Utils.h"

using namespace Utils;

GLUquadric * Utils::circleQuadric = gluNewQuadric();

/*void Utils::drawCircle(float cx, float cy, float r, int num_segments)
{
	glBegin(GL_LINE_LOOP);
	for (int ii = 0; ii < num_segments; ii++)
	{
		float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);

		float x = r * cosf(theta);
		float y = r * sinf(theta);

		glVertex2f(x + cx, y + cy);

	}
	glEnd();
}*/

void Utils::strokeString(const char * str)
{
	while (*str != '\0') {
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *str);
		str++;
	}
}

double Utils::point3dDistance(const point3d &a, const point3d &b)
{
	double xd = a.x - b.x, yd = a.y - b.y, zd = a.z - b.z;
	return sqrt(xd*xd + yd*yd + zd*zd);
}
