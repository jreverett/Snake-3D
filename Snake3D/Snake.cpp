#include "Snake.h"

Snake::Snake(float sX, float sZ, int bodyLength) {
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
	// TODO: this...
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

void Snake::detectCollisions() {
	// TODO: this...
}

Direction Snake::getCurrDirection() { return currDirection; }
