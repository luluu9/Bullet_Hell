#pragma once
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "character.h"
#include "keyboard.h"
#include "entity.h"
#include "GUI.h"


class Game {
public:
	Game();
	~Game();

	void init(const char* title, int width, int height, bool fullscreen);
	
	void handleEvents();
	void update(double delta, double worldTime);
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

	SDL_Texture* textTexture;
	SDL_Surface* textSurface;
	SDL_Surface* charset;
	char text[128];

	Screen* currentScreen;

	KeyboardHandler* keyboard;
};