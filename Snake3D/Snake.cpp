#include "Snake.h"

#include <Windows.h>
#include <mmsystem.h>

Snake::Snake(float sX, float sZ, int bodyLength) {
	if (bodyLength == 0)
		bodyLength = 1; // default to a single snake head

	for (int i = 0; i < bodyLength; i++) {
		SnakeBody* tempBody = new SnakeBody(sX - i, 0, sZ);

		// this is the only block (the head), so make it red
		if (i == 0)
			tempBody->setColour(1, 0, 0);

		body.push_back(tempBody);
	}

	score = 0;
	currDirection = Direction::DIR_RIGHT;
}

void Snake::draw() {
	// draw all the body parts
	for (int i = 0; i <= body.size() - 1; i++) {
		SnakeBody* tempBody = body.at(i);
		tempBody->draw();
	}
}

void Snake::addBody() {
	SnakeBody* tempBody = new SnakeBody(-100, 0, -100);
	body.push_back(tempBody);
}

void Snake::updateSnake() {
	move(currDirection);
}

void Snake::move(Direction direction) {
	SnakeBody* snakeHead = body.at(0);
	
	if (direction == Direction::DIR_UP && currDirection != Direction::DIR_DOWN)	{
		updateBody();
		snakeHead->setZ(snakeHead->getZ() + 1.0f);
		currDirection = direction;
	}
	else if (direction == Direction::DIR_DOWN && currDirection != Direction::DIR_UP) {
		updateBody();
		snakeHead->setZ(snakeHead->getZ() - 1.0f);
		currDirection = direction;
	}
	else if (direction == Direction::DIR_LEFT && currDirection != Direction::DIR_RIGHT) {
		updateBody();
		snakeHead->setX(snakeHead->getX() - 1.0f);
		currDirection = direction;
	}
	else if (direction == Direction::DIR_RIGHT && currDirection != Direction::DIR_LEFT) {
		updateBody();
		snakeHead->setX(snakeHead->getX() + 1.0f);
		currDirection = direction;
	}
}

void Snake::updateBody() {
	// propagate changes along the snakes body
	for (int i = body.size() - 1; i >= 1; i--) {
		SnakeBody* tempBody = body.at(i);
		SnakeBody* prevBody = body.at(i - 1);
		tempBody->setX(prevBody->getX());
		tempBody->setZ(prevBody->getZ());
	}
}

void Snake::detectCollisions(Food* food, int halfGridSize) {
	// check if the snakes head has hit something
	float xHead = body.at(0)->getX();
	float zHead = body.at(0)->getZ();

	int border = halfGridSize + 1;

	if (xHead == border || xHead == -border || zHead == border || zHead == -border) {
		// snake hit the border, kill it
		alive = false;
	}
	else if (food->getX() == xHead && food->getZ() == zHead) {
		// snake hit the food
		PlaySoundW(L"sounds/eating_sound.wav", NULL, SND_ASYNC);

		// 1. add a body section
		this->addBody();

		// 2. move the food to a random location (not under the snake)
		int randX = rand() % (halfGridSize * 2);
		int randZ = rand() % (halfGridSize * 2);

		randX -= halfGridSize;
		randZ -= halfGridSize;
		
		food->setX(randX);
		food->setZ(randZ);
	}
	for (int i = 1; i <= body.size() - 1; i++)	{
		if (xHead == body.at(i)->getX() && zHead == body.at(i)->getZ()) {
			// snake hit itself, kill it
			alive = false;
		}
	}

	if (!alive)
		PlaySoundW(L"sounds/death_sound.wav", NULL, SND_ASYNC);
}

Direction Snake::getCurrDirection() { return currDirection; }

bool Snake::isAlive() { return alive; }
