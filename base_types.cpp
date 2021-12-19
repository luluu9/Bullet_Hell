#include "base_types.h"
#include "math.h"

#define PI 3.14159265


Vector2::Vector2() {
	x = 0;
	y = 0;
}

Vector2::Vector2(float _x, float _y) {
	x = _x;
	y = _y;
}

Vector2 Vector2::normalized() {
	float magnitude = sqrtf(x * x + y * y);
	return Vector2(x / magnitude, y / magnitude);
}

float Vector2::getLength() {
	return sqrtf(x * x + y * y);
}

float Vector2::getAngleTo(Vector2 destVec) {
	return atan2(y-destVec.y, destVec.x - x) * 180 / PI + 180;
}