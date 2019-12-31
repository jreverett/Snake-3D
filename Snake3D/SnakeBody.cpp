#include "SnakeBody.h"
#include "Snake3D.h"

#include <GL/freeglut.h>

SnakeBody::SnakeBody(float sbX, float sbY, float sbZ, float sbSize) {
	x = sbX;
	y = sbY;
	z = sbZ;
	size = sbSize;
	r = red;
	g = green;
	b = blue;
}

void SnakeBody::draw() {
	glPushMatrix();
	glTranslatef(-x, y, z);
	if (r > 1 || g > 1 || b > 1)
		glColor3ub(r, g, b);
	else
		glColor3f(r, g, b);
	glCallList(cube);
	glPopMatrix();
}

void SnakeBody::setColour(float sbR, float sbG, float sbB) {
	r = sbR;
	g = sbG;
	b = sbB;
}

void SnakeBody::setX(float sbX) { x = sbX; }
void SnakeBody::setY(float sbY) { x = sbY; }
void SnakeBody::setZ(float sbZ) { x = sbZ; }

float SnakeBody::getX() { return x; }
float SnakeBody::getY() { return y; }
float SnakeBody::getZ() { return z; }
