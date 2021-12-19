#include "base_types.h"
#include "math.h"

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