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
	double ACCEL = 30;
	double ROT_SPEED = 2;
	double MAX_SPEED = 500; //px per sec
	int ATTACK_ROT_MULTIPLIER = 10;
	int healthPoints = 100;

	//Initializes the variablesA
	Player(SDL_Renderer* _renderer, SDL_Texture* _texture,
		   SDL_Rect* _camera, KeyboardHandler* _keyboard,
		   GameEntities* _entities);

	void handleEvent(SDL_Event& event);
	void update(double delta);
	void collide(Entity* collidingEntity);
	void startInvincibility();
	void stopInvincibility();

private:
	KeyboardHandler* keyboard;
	GameEntities* entities;
	float invincibleTimer = 0;
	int invincibleTime = 2500; //msec
	bool invincible = false;

};


class Chemiczny : public Enemy {
public:
	Chemiczny(SDL_Renderer* _renderer, SDL_Texture* _texture, 
			  SDL_Rect* _camera, GameEntities* _entities, Player* player,
			  SDL_Texture* _AcidTexture);

	void Chemiczny::updatePosition(double delta);
	void Chemiczny::tryToShoot(double delta);
	void Chemiczny::update(double delta);

private:
	int shootingThreshold = 600; // px
	int shootingDelay = 1500; // in msec
	int shootingTimer = shootingDelay;
};