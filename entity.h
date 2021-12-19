#pragma once
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "base_types.h"


class Entity {
public:
	int WIDTH;
	int HEIGHT;
	bool colliding;

	Entity(SDL_Renderer* _renderer, SDL_Texture* _texture, SDL_Rect* _camera);

	virtual void handleEvent(SDL_Event& event);
	virtual void update(double delta);
	void render();
	Vector2 getPos();
	void setPos(Vector2 newPos);
	SDL_Rect getRect();
	SDL_Rect getGlobalRect();
	void setAngle(int angle);

protected:
	Vector2 pos;
	int speed, angle;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_Rect* camera;
};


struct GameEntities {
	unsigned int amount = 10;
	unsigned int currentEntity = 0;
	Entity** entities = new Entity * [amount];

	void addEntity(Entity* entity) {
		if (currentEntity >= amount) {
			// resize array
			amount *= 2;
			Entity** extendedArray = new Entity * [amount];
			for (int i = 0; i < currentEntity; i++)
				extendedArray[i] = entities[i];
			delete[] entities;
			entities = extendedArray;
		}
		entities[currentEntity] = entity;
		currentEntity++;
	}

	void removeEntity(Entity* entityToRemove) {
		Entity* firstEncounteredEntity = nullptr;
		unsigned int firstEncounteredEntityId = amount;
		for (int i = amount - 1; i > 0; i--) {
			if (entities[i] != nullptr) {
				if (entities[i] == entityToRemove) {
					entities[i]->~Entity(); // call destructor
					entities[i] = nullptr;
					if (firstEncounteredEntity) { // fill gap in array
						entities[i] = firstEncounteredEntity;
						entities[firstEncounteredEntityId] = nullptr;
					}
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


class Enemy : public Entity {
public:
	double ACCEL = 30;
	double SPEED = 50;
	double ROT_SPEED = 2;
	double MAX_SPEED = 500; //px per sec

	//Initializes the variablesA
	Enemy(SDL_Renderer* _renderer, SDL_Texture* _texture,
		  SDL_Rect* _camera, GameEntities* entities);

	void update(double delta);

protected:
	GameEntities* entities;
};