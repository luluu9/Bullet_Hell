#pragma once
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "base_types.h"
#include <stdio.h>

enum ENTITY_TYPES { UNKNOWN, PLAYER, ENEMY, WEAPON };


struct GameEntities; // forward declaration
class Player; // forward declaration

class Entity {
public:
	int WIDTH;
	int HEIGHT;
	bool colliding;
	int entityType;

	Entity(
		SDL_Renderer* _renderer, 
		char* texturePath, 
		SDL_Rect* _camera);
	~Entity();

	virtual void handleEvent(SDL_Event& event);
	virtual void update(double delta);
	virtual void render();
	Vector2 getPos();
	void setPos(Vector2 newPos);
	Vector2 getGlobalPos();
	SDL_Rect getRect();
	SDL_Rect getGlobalRect();
	int getAngle();
	void setAngle(int _angle);

protected:
	Vector2 pos;
	int speed, angle;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_Rect* camera;
};


class DestroyableEntity : public Entity {
public:
	DestroyableEntity(
		SDL_Renderer* _renderer, 
		char* texturePath,
		SDL_Rect* _camera);
	void render();
	void drawHPBar();
	void setHP(float _MAX_HP);
protected:
	float MAX_HP = 100;
	float healthPoints = MAX_HP;
};


class Enemy : public DestroyableEntity {
public:
	double ACCEL = 30;
	double SPEED = 50;
	double ROT_SPEED = 2;
	double MAX_SPEED = 500; //px per sec

	//Initializes the variablesA
	Enemy(SDL_Renderer* _renderer, 
		char* texturePath,
		SDL_Rect* _camera, 
		GameEntities* entities,
		Player* _player);

	void hit(float damage);

protected:
	GameEntities* entities;
	Player* player;
};


struct GameEntities {
public:
	unsigned int currentEntity = 0;

	GameEntities();
	~GameEntities();

	void removeQueuedEntities();
	Entity* getEntity(unsigned int entityId);
	void addEntity(Entity* entity);
	void queueRemove(Entity* entityToRemove);

private:
	unsigned int amount = 10;
	unsigned int queueAmount = 10;
	unsigned int currentQueueEntity = 0;
	Entity** entities = new Entity * [amount];
	Entity** removeQueue = new Entity * [amount];

	void removeEntity(Entity* entityToRemove);
};

bool isColliding(Entity* a, Entity* b);