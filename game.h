#pragma once
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "character.h"
#include "keyboard.h"

struct GameEntities {
	unsigned int amount = 10;
	unsigned int currentEntity = 0;
	Entity** entities = new Entity*[amount];
	
	void addEntity(Entity* entity) {
		if (currentEntity < amount)
			entities[currentEntity] = entity;
		else {
			// implement resizing array
		}
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
				if (firstEncounteredEntity == nullptr){ 
					firstEncounteredEntity = entities[i];
					firstEncounteredEntityId = i;
				}
			}
		}
	}

};


class Game {
public:
	Game();
	~Game();

	void init(const char* title, int width, int height, bool fullscreen);
	
	void handleEvents();
	void update(double delta);
	void render();
	void clean();

	bool running();

	SDL_Renderer* Game::getRenderer();
	SDL_Texture* loadTextureFromBMP(const char* filepath);

private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	
	SDL_Texture* eti;
	SDL_Texture* background;
	GameEntities entities;
	Player* player;
	SDL_Rect camera;

	KeyboardHandler* keyboard;
};