#pragma once
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "keyboard.h"
#include "weapons.h"
#include "entity.h"
#include "base_types.h"
#include "characters.h"


class Player : public DestroyableEntity {
public:
	double ACCEL = 30;
	double ROT_SPEED = 2;
	double MAX_SPEED = 700; //px per sec
	int INVINCIBLE_TIME = 00;
	int MAX_ATTACK_TIME = 2000;
	int ATTACK_ROT_MULTIPLIER = 10;

	//Initializes the variablesA
	Player(
		SDL_Renderer* _renderer,
		SDL_Rect* _camera,
		KeyboardHandler* _keyboard,
		GameEntities* _entities);
	~Player();

	void handleEvent(SDL_Event& event);
	void attack();
	void update(double delta);
	void render();
	void collide(Entity* collidingEntity, double delta);
	void collideEnemy(Enemy* enemy, double delta);
	void collideWeapon(Weapon* weapon, double delta);
	void startInvincibility();
	void stopInvincibility();

private:
	KeyboardHandler* keyboard;
	GameEntities* entities;
	Timer* invincibleTimer;
	Timer* attackTimer;
	Timer* attackCountdown;
	bool invincible = false;
	bool attacking = false;

	float damage = 100; // dmg per second
};


class Spark : public Entity {
public:
	Spark(
		SDL_Renderer* _renderer,
		SDL_Rect* _camera,
		GameEntities* entities,
		Vector2 startPos);
	void update(double delta);
	void render();

private:
	GameEntities* entities;
	int MAX_SPEED = 500;
	int MIN_SPEED = 200;
	int MAX_ANGLE = 360;
	int MAX_RAND_POS = 30;
	float SCALE_DECREASE = (float)0.02;
	double speed;
	float scale;
};


class Chemiczny : public Enemy {
public:
	Chemiczny(
		SDL_Renderer* _renderer,
		SDL_Rect* _camera,
		GameEntities* _entities,
		Player* player);

	void Chemiczny::updatePosition(double delta);
	void Chemiczny::tryToShoot(double delta);
	void Chemiczny::update(double delta);

private:
	Timer* shootingTimer;
	int SHOOTING_THRESHOLD = 500; // px
	int SHOOTING_DELAY = 1000; // in msec
	
};


class AIR : public Enemy {
public:
	AIR(
		SDL_Renderer* _renderer,
		SDL_Rect* _camera,
		GameEntities* _entities,
		Player* player);
	AIR::~AIR();

	Vector2 AIR::getRobotPos(int angle);
	void AIR::setPos(Vector2 newPos);
	void AIR::updatePosition(double delta);
	void AIR::tryToShoot(double delta);
	void AIR::update(double delta);
private:
	int SPEED = 200;
	int SHOOTING_DELAY = 2500; // in msec
	int ROBOTS_NUMBER = 5;
	int ROBOTS_RADIUS = 250;
	int SHOOTING_THRESHOLD = 600;
	Timer* shootingTimer;
	Robot** robots;
	
};


class WILIS : public Enemy {
public:
	WILIS(
		SDL_Renderer* _renderer,
		SDL_Rect* _camera,
		GameEntities* _entities,
		Player* player);

	void updatePosition(double delta);
	void tryToShoot(double delta);
	void update(double delta);
private:
	Timer* shootingTimer;
	int SHOOTING_THRESHOLD = 400; // px
	int SHOOTING_DELAY = 2500; // in msec
	int SPEED = 200;

};