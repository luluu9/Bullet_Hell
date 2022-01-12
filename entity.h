#pragma once
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "base_types.h"
#include <stdio.h>

enum ENTITY_TYPES { UNKNOWN, PLAYER, ENEMY, WEAPON };
enum ENEMY_TYPES { CHEMICZNY=1, AIR_TYPE, WILIS_TYPE };
enum WEAPON_TYPES { ACID=1, ROBOT, EMP_GRANADE, HAMMER, WRECKING_BALL, GAS };


struct GameEntities; // forward declaration
class Player; // forward declaration

class Entity {
public:
	int WIDTH;
	int HEIGHT;
	bool COLLISIONS_DISABLED = false;
	bool colliding;
	int entityType;

	Entity(
		SDL_Renderer* _renderer,
		char* texturePath,
		Camera* _camera);
	Entity(
		SDL_Renderer* _renderer,
		Camera* _camera);
	~Entity();

	virtual void handleEvent(SDL_Event& event);
	virtual void update(double delta);
	virtual void render();
	void renderCollisionBox();
	Vector2 getPos();
	void setPos(Vector2 newPos);
	Vector2 getGlobalPos();
	SDL_Rect getRect();
	SDL_Rect getGlobalRect();
	int getAngle();
	void setAngle(int _angle);

protected:
	Vector2 pos;
	float speed, angle;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	Camera* camera;
};


class DestroyableEntity : public Entity {
public:
	DestroyableEntity(
		SDL_Renderer* _renderer,
		char* texturePath,
		Camera* _camera);
	void render();
	void drawHPBar();
	void setHP(float _MAX_HP);
	float getHP();
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
	int enemyType = UNKNOWN;

	//Initializes the variablesA
	Enemy(SDL_Renderer* _renderer,
		char* texturePath,
		Camera* _camera,
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


class AnimationPlayer : public Entity {
public:
	AnimationPlayer(
		SDL_Renderer* _renderer,
		Camera* _camera,
		char* _texturesDir,
		unsigned int _frames,
		Vector2 _pos,
		GameEntities* _entities);
	~AnimationPlayer();
	void update(double delta);
	void render();
	void nextFrame();
	void start();
private:
	SDL_Texture** txtFrames;
	GameEntities* entities;
	Timer* timer;
	char* texturesDir;
	int frames;
	int currentFrame = 0;
	float speed = 50; // miliseconds per frame
	bool started = false;
};

bool isColliding(Entity* a, Entity* b);
Vector2 getDirectionFromAngle(float angle);