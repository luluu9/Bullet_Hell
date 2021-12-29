#pragma once
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "base_types.h"
enum ENTITY_TYPES { UNKNOWN, PLAYER, ENEMY, WEAPON };

struct GameEntities; // forward declaration
class Player; // forward declaration

class Entity {
public:
	int WIDTH;
	int HEIGHT;
	bool colliding;
	int entityType;

	Entity(SDL_Renderer* _renderer, SDL_Texture* _texture, SDL_Rect* _camera);

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
	DestroyableEntity(SDL_Renderer* _renderer, SDL_Texture* _texture,
		SDL_Rect* _camera);
	void render();
	void drawHPBar();
protected:
	float MAX_HP = 1000;
	float healthPoints = MAX_HP;
};


class Enemy : public DestroyableEntity {
public:
	double ACCEL = 30;
	double SPEED = 50;
	double ROT_SPEED = 2;
	double MAX_SPEED = 500; //px per sec

	//Initializes the variablesA
	Enemy(SDL_Renderer* _renderer, SDL_Texture* _texture,
		SDL_Rect* _camera, GameEntities* entities,
		Player* _player, SDL_Texture* _weaponTexture);

	void hit(float damage);

protected:
	GameEntities* entities;
	Player* player;
	SDL_Texture* weaponTexture;
};




class Spark : public Entity {
public:
	Spark(SDL_Renderer* _renderer, SDL_Texture* _texture,
		SDL_Rect* _camera, GameEntities* entities, Vector2 startPos);
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


// todo: move functions to .cpp
struct GameEntities {
	unsigned int currentEntity = 0;

	GameEntities() {
		for (unsigned int i = 0; i < amount; i++)
			entities[i] = nullptr;
		for (unsigned int i = 0; i < queueAmount; i++)
			removeQueue[i] = nullptr;
	}

	void removeQueuedEntities() {
		// remove all entities queued to remove
		for (unsigned int i = 0; i < currentQueueEntity; i++) {
			removeEntity(removeQueue[i]);
			removeQueue[i] = nullptr;
		}
		currentQueueEntity = 0;
	}

	Entity* getEntity(unsigned int entityId) {
		if (entityId >= amount) return nullptr;
		return entities[entityId];
	}

	void addEntity(Entity* entity) {
		if (currentEntity >= amount) {
			// resize array
			amount *= 2;
			Entity** extendedArray = new Entity * [amount];
			for (unsigned int i = 0; i < currentEntity; i++)
				extendedArray[i] = entities[i];
			for (unsigned int i = currentEntity; i < amount; i++)
				extendedArray[i] = nullptr;
			delete[] entities;
			entities = extendedArray;
		}
		entities[currentEntity] = entity;
		currentEntity++;
	}

	void queueRemove(Entity* entityToRemove) {
		if (currentQueueEntity >= queueAmount) {
			// resize array
			queueAmount *= 2;
			Entity** extendedArray = new Entity * [queueAmount];
			for (unsigned int i = 0; i < currentQueueEntity; i++)
				extendedArray[i] = removeQueue[i];
			for (unsigned int i = currentQueueEntity; i < queueAmount; i++)
				extendedArray[i] = nullptr;
			delete[] removeQueue;
			removeQueue = extendedArray;
		}
		removeQueue[currentQueueEntity] = entityToRemove;
		currentQueueEntity++;
	}

private:
	unsigned int amount = 10;
	unsigned int queueAmount = 10;
	unsigned int currentQueueEntity = 0;
	Entity** entities = new Entity * [amount];
	Entity** removeQueue = new Entity * [amount];

	void removeEntity(Entity* entityToRemove) {
		Entity* firstEncounteredEntity = nullptr;
		unsigned int firstEncounteredEntityId = amount;
		for (int i = amount - 1; i >= 0; i--) {
			if (entities[i] != nullptr) {
				if (entities[i] == entityToRemove) {
					entities[i]->~Entity(); // call destructor
					entities[i] = nullptr;
					if (firstEncounteredEntity != nullptr) { // fill gap in array
						entities[i] = firstEncounteredEntity;
						entities[firstEncounteredEntityId] = nullptr;
					}
					currentEntity--;
					break;
				}
				if (firstEncounteredEntity == nullptr) {
					firstEncounteredEntity = entities[i];
					firstEncounteredEntityId = i;
				}
			}
		}
	}
};

bool isColliding(Entity* a, Entity* b);