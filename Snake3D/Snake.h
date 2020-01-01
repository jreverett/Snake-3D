#ifndef SNAKE_H
#define SNAKE_H

#include "Snake3D.h"
#include "SnakeBody.h"

#include <vector>

class Snake {
public:
	std::vector<SnakeBody*> body; // array of bodyparts :O

	Snake(float sX, float sZ, int bodyLength);
	
	void draw();
	void addBody();
	void updateSnake(); // steps the entire snake
	void move(Direction direction);
	void updateBody(); // moves the rest of the snake (i.e not the head)
	void detectCollisions();

	Direction getCurrDirection();

private:
	int score;
	Direction currDirection;
};

#endif