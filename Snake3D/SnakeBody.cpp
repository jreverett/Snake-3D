#include "SnakeBody.h"
#include "Snake3D.h"

#include <GL/freeglut.h>

SnakeBody::SnakeBody(float sbX, float sbY, float sbZ) {
	x = sbX;
	y = sbY;
	z = sbZ;
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

    #pragma region cube_vectors
    glBegin(GL_QUADS);
    // front face
    glNormal3f(0, 0, -1);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);
    // back face
    glNormal3f(0, 0, 1);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    // top face
    glNormal3f(0, 1, 0);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, -0.5);
    // bottom face
    glNormal3f(0, -1, 0);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, -0.5);
    // left face
    glNormal3f(1, 0, 0);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, -0.5);
    // right face
    glNormal3f(-1, 0, 0);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(-0.5, -0.5, -0.5);
    glEnd();
    #pragma endregion cube_vectors

	glPopMatrix();
}

void SnakeBody::setColour(float sbR, float sbG, float sbB) {
	r = sbR;
	g = sbG;
	b = sbB;
}

void SnakeBody::setX(float sbX) { x = sbX; }
void SnakeBody::setY(float sbY) { y = sbY; }
void SnakeBody::setZ(float sbZ) { z = sbZ; }

float SnakeBody::getX() { return x; }
float SnakeBody::getY() { return y; }
float SnakeBody::getZ() { return z; }
