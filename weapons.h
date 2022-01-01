#pragma once
#include "entity.h"

class Weapon : public Entity {
public:
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
		int startAngle,
		float _radius,
		int _rotateSpeed);
	void update(double delta);
private:
	float radius; 
	int rotateSpeed; // per second
};


class EMP : public Weapon {
public:
	EMP(
		SDL_Renderer* _renderer,
		char* texturePath,
		SDL_Rect* _camera,
		GameEntities* _entities,
		float _startAngle);
	void update(double delta);
	void render();
	void startAnimation();
private:
	const int GRANADE_SPEED = 300;
	const int ROTATE_SPEED = 300;
	const int BOOM_DELAY = 1500; // msec
	const int BOOM_DIRECTIONS = 4;
	const int BOOM_DURATION = 0.5;
	Timer boomTimer = Timer(BOOM_DELAY);
	GameEntities* entities;
	float angle = 0;
	float startAngle;
	
	
};

class EMPWave : public Weapon {
public:
	EMPWave(
		SDL_Renderer* _renderer,
		char* texturePath,
		SDL_Rect* _camera,
		GameEntities* _entities,
		float _startAngle);
	void update(double delta);
	void render();
private:
	const int WAVE_SPEED = 300;
	const int WAVE_DURATION = 1500; // msec;
	GameEntities* entities;
	double scale = 0.1;
};