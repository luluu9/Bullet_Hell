#pragma once
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "keyboard.h"
#include "weapons.h"
#include "entity.h"
#include "base_types.h"
#include "character.h"


class Player : public DestroyableEntity {
public:
	double ACCEL = 30;
	double ROT_SPEED = 2;
	double MAX_SPEED = 700; //px per sec
	int INVINCIBLE_TIME = 2500;
	int MAX_ATTACK_TIME = 2000;
	int ATTACK_ROT_MULTIPLIER = 10;


	//Initializes the variablesA
	Player(SDL_Renderer* _renderer, SDL_Texture* _texture,
		   SDL_Rect* _camera, KeyboardHandler* _keyboard,
		   GameEntities* _entities, SDL_Texture* _collisionTexture);

	void handleEvent(SDL_Event& event);
	void attack();
	void update(double delta);
	void render();
	void collide(Entity* collidingEntity, double delta);
	void startInvincibility();
	void stopInvincibility();

private:
	KeyboardHandler* keyboard;
	GameEntities* entities;
	SDL_Texture* collisionTexture;
	Timer* invincibleTimer;
	Timer* attackTimer;
	Timer* attackCountdown;
	bool invincible = false;
	bool attacking = false;
	
	float damage = 100; // dmg per second

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
	//float MAX_HP = 1000;
};