#ifndef FOOD_H
#define FOOD_H

enum class FoodDirection {
	FD_UP,
	FD_DOWN
};

class Food {
public:
	Food(float fX, float fY, float fZ);

	void draw();

	float setX(float fX);
	float setY(float fY);
	float setZ(float fZ);
	float getX();
	float getY();
	float getZ();
	float getDegrees();
private:
	float x, y, z, degrees;
	FoodDirection foodDir = FoodDirection::FD_UP;
};

#endif