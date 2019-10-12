#include <Windows.h>
#include <GL/glut.h>

#include <thread>
#include <chrono>
#include <iostream>
#include <vector>
#include <random>

#include "Plant.h"
#include "Human.h"
#include "VisualObject.h"
#include "World.h"

GLfloat elevation = 10.0f;
bool keyDown = false;
bool keyUp = false;
std::thread * logicThread;
std::chrono::time_point<std::chrono::high_resolution_clock> logicLast;
GLfloat aspectRatio;
std::chrono::milliseconds logicStepDuration = std::chrono::milliseconds(10000);
int stepMultiplier = 10;
World world;
int numFrames = 0;
int fps;
std::chrono::time_point<std::chrono::high_resolution_clock> frameLast;

void render() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	glScalef(1.0f / elevation, 1.0f / elevation, 1.0f / elevation);
	world.render();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslated(-aspectRatio, 0.0l, 0.0l);
	glScalef(0.001f, 0.001f, 0.001f);
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 'a');
	glPopMatrix();

	glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height) {
	if (height == 0) height = 1;
	aspectRatio = (GLfloat)width / (GLfloat)height;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset the projection matrix
	if (width >= height) {
		// aspect >= 1, set the height from -1 to 1, with larger width
		gluOrtho2D(-1.0 * aspectRatio, 1.0 * aspectRatio, -1.0, 1.0);
	}
	else {
		// aspect < 1, set the width to -1 to 1, with larger height
		gluOrtho2D(-1.0, 1.0, -1.0 / aspectRatio, 1.0 / aspectRatio);
	}
}

void specialInput(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_DOWN:
		keyDown = true;
		break;
	case GLUT_KEY_UP:
		keyUp = true;
		break;
	}
}

void specialUpInput(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_DOWN:
		keyDown = false;
		break;
	case GLUT_KEY_UP:
		keyUp = false;
		break;
	}
}

void stepLogic() {
	bool updateRequired = false;
	if (keyDown) {
		elevation += 0.2f;
		updateRequired = true;
	}
	if (keyUp) {
		elevation -= 0.2f;
		if (elevation < 1.0f) elevation = 1.0f;
		updateRequired = true;
	}
	for (int i = 0; i < stepMultiplier; i++) world.stepLogic(logicStepDuration);
	if (updateRequired) {
		glutPostRedisplay();
	}
}

void logicLoop() {
	while (true) {
		auto now = std::chrono::high_resolution_clock::now();
		auto delta = now - logicLast;
		if (delta > std::chrono::milliseconds(200)) {
			logicLast = now;
			std::cout << "Frameskip" << std::endl;
		}

		while (now > logicLast) {
			logicLast += std::chrono::milliseconds(20);
			stepLogic();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}

void idle() {
	glutPostRedisplay();
	std::this_thread::yield();
}

int main(int argc, char ** argv)
{
	world.addVisualObject(new Human(0.0, 0.0, 0.0, &world));
	world.addVisualObject(new Human(10.0, 10.0, 0.0, &world));
	for (int i = 0; i < 1000; i++) {
		Plant* ptr = new Plant((rand()%1000)/10.0-50.0, (rand()%1000)/10.0-50.0, 0.0, &world);
		world.addVisualObject(ptr);
	}
	glutInit(&argc, argv);
	glutInitWindowSize(800, 640);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Simulator");
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialInput);
	glutSpecialUpFunc(specialUpInput);
	glutIdleFunc(idle);
	logicLast = std::chrono::high_resolution_clock::now();
	logicThread = new std::thread(logicLoop);
	glutMainLoop();
}

