#pragma once
#include "entity.h"

class Weapon : public Entity {
public:
	int WIDTH = 40;
	int HEIGHT = 40;
	double ACCEL = 30;
	double SPEED = 300;

	Weapon(SDL_Renderer* _renderer, SDL_Texture* _texture, SDL_Rect* _camera, int _angle);
	void update(double delta);
};

