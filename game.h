#pragma once
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "characters.h"
#include "keyboard.h"
#include "entity.h"
#include "GUI.h"

#ifndef SCREEN_ENUM
#define SCREEN_ENUM
enum SCREEN { MAIN_MENU, LEVEL_1, LEVEL_2, LEVEL_3, QUIT };
#endif

class Screen; // forward declaration


class Game {
public:
	Game();
	~Game();

	void init(const char* title, int width, int height, bool fullscreen);

	void handleEvents();
	void update(double delta, double worldTime);
	void render();
	void clean();
	void changeScreen(SCREEN screenId);

	bool running();

	SDL_Renderer* Game::getRenderer();
	SDL_Texture* loadTextureFromBMP(const char* filepath);
	KeyboardHandler* getKeyboard();

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