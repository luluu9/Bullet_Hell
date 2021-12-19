#pragma once
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "keyboard.h"
#include "weapons.h"
#include "entity.h"
#include "base_types.h"
#include "character.h"


class Player : public Entity {
public:
	int WIDTH = 40;
	int HEIGHT = 40;
	double ACCEL = 30;
	double ROT_SPEED = 2;
	double MAX_SPEED = 500; //px per sec
	int ATTACK_ROT_MULTIPLIER = 10;

	//Initializes the variablesA
	Player(SDL_Renderer* _renderer, SDL_Texture* _texture, SDL_Rect* _camera, KeyboardHandler* _keyboard);

	void handleEvent(SDL_Event& event);
	void update(double delta);
	void collide(Entity* collidingEntity);
	void collide(Weapon* collidingEntity);

private:
	KeyboardHandler* keyboard;
};


class Enemy : public Entity {
public:
	int WIDTH = 40;
	int HEIGHT = 40;
	double ACCEL = 30;
	double SPEED = 50;
	double ROT_SPEED = 2;
	double MAX_SPEED = 500; //px per sec
	
	//Initializes the variablesA
	Enemy(SDL_Renderer* _renderer, SDL_Texture* _texture, SDL_Rect* _camera);

	void update(double delta);
};
