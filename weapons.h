#pragma once
#include "entity.h"

class Weapon : public Entity {
public:
	int WIDTH = 40;
	int HEIGHT = 40;
	double ACCEL = 30;
	double SPEED = 300;
	int DAMAGE = 10;

	Weapon(
		SDL_Renderer* _renderer, 
		char* texturePath,
		SDL_Rect* _camera, 
		int _angle);
	void update(double delta);
};

