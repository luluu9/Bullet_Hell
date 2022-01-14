#pragma once
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "keyboard.h"
#include "weapons.h"
#include "entity.h"
#include "base_types.h"
#include "characters.h"
#include "settings.h"
#include "GUI.h"


class ScoreCounter;

class Player : public DestroyableEntity {
public:
	double ACCEL = 20;
	double ROT_SPEED = 2;
	double MAX_SPEED = 700; //px per sec
	int INVINCIBLE_TIME = 1500;
	int MAX_ATTACK_TIME = 2000;
	int ATTACK_ROT_MULTIPLIER = 10;
	int HIT_SHAKE_DURATION = 100;
	int HIT_SHAKE = 10;
	int BONUS_HP = 25;
	int BONUS_MARGIN = SCREEN_HEIGHT/4;

	//Initializes the variablesA
	Player(
		SDL_Renderer* _renderer,
		Camera* _camera,
		KeyboardHandler* _keyboard,
		GameEntities* _entities,
		ScoreCounter* score);
	~Player();

	void handleEvent(SDL_Event& event);
	void attack();
	void update(double delta);
	void renderEnergy();
	void render();
	void collide(Entity* collidingEntity, double delta);
	void collideEnemy(Enemy* enemy, double delta);
	void collideWeapon(Weapon* weapon, double delta);
	void startInvincibility();
	void stopInvincibility();
	void addBonus();

private:
	KeyboardHandler* keyboard;
	GameEntities* entities;
	Timer* invincibleTimer;
	Timer* attackTimer;
	Timer* attackCountdown;
	bool invincible = false;
	bool attacking = false;
	bool gassed = false;
	float attackStartAngle = 0;
	ScoreCounter* score;

	float damage = 100; // dmg per second
};


class Spark : public Entity {
public:
	Spark(
		SDL_Renderer* _renderer,
		Camera* _camera,
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
		Camera* _camera,
		GameEntities* _entities,
		Player* player);

	void Chemiczny::updatePosition(double delta);
	void Chemiczny::tryToShoot(double delta);
	void Chemiczny::gasOut();
	void Chemiczny::updateGas(double delta=0.0);
	void Chemiczny::update(double delta);
private:
	Timer* shootingTimer;
	Timer* gasTimer;
	Timer* addGasTimer;
	int SPEED = CHEMICZNY_SPEED;
	int SHOOTING_THRESHOLD = CHEMICZNY_SHOOTING_THRESHOLD; // px
	int SHOOTING_DELAY = CHEMICZNY_SHOOTING_DELAY; // in msec
	int SHOOTING_SIDES = CHEMICZNY_SHOOTING_SIDES;
	int GAS_DELAY = CHEMICZNY_GAS_DELAY; // in msec
	int GAS_SPEED = CHEMICZNY_GAS_SPEED; // how fast gas expands to fill GAS_WIDTH 
	int GAS_HEIGHT = CHEMICZNY_GAS_HEIGHT;
	int GAS_WIDTH = CHEMICZNY_GAS_WIDTH;
	int GAS_WEAPON_WIDTH;
	int GAS_WEAPON_HEIGHT;
	Vector2 gasStartPos;
	int addGasTime;
	float gasEjected = 0;
	int gasNumberToFillHeight = 0;
	
};


class AIR : public Enemy {
public:
	AIR(
		SDL_Renderer* _renderer,
		Camera* _camera,
		GameEntities* _entities,
		Player* player);
	AIR::~AIR();

	Vector2 AIR::getRobotPos(int angle);
	void AIR::setPos(Vector2 newPos);
	void AIR::updatePosition(double delta);
	void AIR::tryToShoot(double delta);
	void AIR::update(double delta);
private:
	int SPEED = AIR_SPEED;
	int SHOOTING_DELAY = AIR_SHOOTING_DELAY;
	int ROBOTS_NUMBER = AIR_ROBOTS_NUMBER; 
	int ROBOTS_RADIUS = AIR_ROBOTS_RADIUS; 
	int SHOOTING_THRESHOLD = AIR_SHOOTING_THRESHOLD; 
	Timer* shootingTimer;
	Robot** robots;
	
};


class WILIS : public Enemy {
public:
	WILIS(
		SDL_Renderer* _renderer,
		Camera* _camera,
		GameEntities* _entities,
		Player* player);

	void updatePosition(double delta);
	void tryToShoot(double delta);
	void update(double delta);
private:
	Timer* hammerShootingTimer;
	Timer* ballShootingTimer;
	int HAMMER_SHOOTING_THRESHOLD = WILIS_HAMMER_SHOOTING_THRESHOLD; 
	int HAMMER_SHOOTING_DELAY = WILIS_HAMMER_SHOOTING_DELAY;
	int BALL_SHOOTING_DELAY = WILIS_BALL_SHOOTING_DELAY;
	int SPEED = WILIS_SPEED;

};