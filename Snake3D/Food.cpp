#include "Food.h"

#include <GL/freeglut.h>

Food::Food(float fX, float fY, float fZ) {
	x = fX;
	y = fY;
	z = fZ;
	degrees = 0.0f;
}

void Food::draw() {
	// the food should slowly rotate and move up and down (to entice the snake)
	if (y <= 0.2)
		foodDir = FoodDirection::FD_UP;
	else if (y >= 0.6)
		foodDir = FoodDirection::FD_DOWN;

	if (foodDir == FoodDirection::FD_UP)
		y += 0.0001;
	else if (foodDir == FoodDirection::FD_DOWN)
		y -= 0.0001;

	degrees += 0.005f;

	glPushMatrix();
	glTranslatef(-x, y, z);
	glRotatef(degrees, 0, 1, 0);
	
	float offset = 0.5;
	glColor3f(1.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	glVertex3f(0.5f - offset, 0.0f - offset, -0.5f);
	glVertex3f(0.0f - offset, 0.0f - offset, 0.0f);
	glVertex3f(0.5f - offset, 0.0f - offset, 0.5f);
	glVertex3f(1.0f - offset, 0.0f - offset, 0.0f);
	glEnd();

	glPopMatrix();
}

float Food::setX(float fX) { return x = fX; }
float Food::setY(float fY) { return y = fY; }
float Food::setZ(float fZ) { return z = fZ; }

float Food::getX() { return x; }
float Food::getY() { return y; }
float Food::getZ() { return z; }
float Food::getDegrees() { return degrees; }
