#ifndef SNAKE_H
#define SNAKE_H

#include "Snake3D.h"
#include "SnakeBody.h"
#include "Food.h"

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
	void detectCollisions(Food* food, int halfGridSize);

	Direction getCurrDirection();
	bool isAlive(); // checks if the snake has crashed into itself or a wall

private:
	bool randomiseFoodPos(Food* food, int halfGridSize);

	int score;
	bool alive = true;
	Direction currDirection;
};

#endif