#ifndef SNAKEBLOCK_H
#define SNAKEBLOCK_H

// Snake body RGB values:
const int red = 240;
const int green = 150;
const int blue = 50;

class SnakeBody {
public:
	SnakeBody(float sbX, float sbY, float sbZ, float sbSize = 1.0f);

	void draw();

	// getters and setters (RGB & XYZ)
	void setColour(float sbR, float sbG, float sbB);
	void setX(float sbX);
	void setY(float sbY);
	void setZ(float sbZ);
	float getX();
	float getY();
	float getZ();

private:
	float x, y, z, size, r, g, b;
};

#endif