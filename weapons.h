#pragma once
#include "entity.h"

class Weapon : public Entity {
public:
	int WIDTH = 40;
	int HEIGHT = 40;
	double ACCEL = 30;
	double SPEED = 300;
	int DAMAGE = 10;
	int weaponType = UNKNOWN;

	Weapon(
		SDL_Renderer* _renderer, 
		char* texturePath,
		SDL_Rect* _camera, 
		int _angle);
	void update(double delta);
};


class Acid : public Weapon {
public:
	Acid(
		SDL_Renderer* _renderer,
		char* texturePath,
		SDL_Rect* _camera,
		int _angle);
};


class Robot : public Weapon {
public:
	Robot(
		SDL_Renderer* _renderer,
		char* texturePath,
		SDL_Rect* _camera,
		Vector2 startPos,
		int robotId);
	void update(double delta);
private:
	float radius = 200;
	int rotateSpeed = 90; // per second
};


class EMP : public Weapon {
public:
	EMP(
		SDL_Renderer* _renderer,
		char* texturePath,
		SDL_Rect* _camera,
		float _startAngle);
	void update(double delta);
private:
	const int GRANADE_SPEED = 300;
	const int ROTATE_SPEED = 300;
	float angle = 0;
	float startAngle;
	
};

