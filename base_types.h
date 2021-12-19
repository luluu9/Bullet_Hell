#pragma once



struct Vector2 {
	float x;
	float y;

	Vector2();
	Vector2(float _x, float _y);
	Vector2 normalized();
	float getLength();
	float getAngleTo(Vector2 destVec);
	float getDistanceTo(Vector2 destVec);
};