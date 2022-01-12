#include <stdio.h>
#include <cstdlib>
#include "game.h"
#include "drawing.h"
#include "entity.h"
#include "string.h"
#include "GUI.h"
#include "settings.h"


Game::Game() {
	isRunning = true;
	window = nullptr;
	renderer = nullptr;
	textTexture = nullptr;
	textSurface = nullptr;
	charset = nullptr;
	keyboard = nullptr;
	currentScreen = nullptr;
	score = nullptr;

	int flags = 0;
	if (FULLSCREEN)
		flags = SDL_WINDOW_FULLSCREEN;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf_s("SDL_Init error: %s\n", SDL_GetError());
		isRunning = false;
		return;
	}

	int rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, flags, &window, &renderer);
	if (rc != 0) {
		SDL_Quit();
		sprintf_s("SDL_CreateWindowAndRenderer error: %s\n", 39*sizeof(char), SDL_GetError());
		isRunning = false;
		return;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	SDL_SetWindowTitle(window, TITLE);
	//SDL_ShowCursor(SDL_DISABLE);

	// prepare everything to draw text
	textSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	textTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetTextureBlendMode(textTexture, SDL_BLENDMODE_BLEND);

	charset = SDL_LoadBMP(CHARSET_TXT_PATH);
	SDL_SetSurfaceBlendMode(charset, SDL_BLENDMODE_NONE);
	SDL_SetColorKey(charset, true, 0x0000000);

	keyboard = new KeyboardHandler;
	score = new ScoreCounter;
	currentScreen = new MainMenu(renderer, textSurface, charset, this);
}

Game::~Game() {
	clean();
}

void Game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false;
			if (event.key.keysym.sym == SDLK_n) changeScreen(LEVEL_1);
			break;
		case SDL_QUIT:
			isRunning = false;
			break;
		};
		keyboard->handleEvent(event);
		if (currentScreen != nullptr)
			currentScreen->handleEvent(event);
	};
}

void Game::update(double delta, double worldTime) {
	currentScreen->update(delta, worldTime);
}

void Game::render() {
	SDL_RenderClear(renderer);
	if (currentScreen!=nullptr)
		currentScreen->render();
	SDL_UpdateTexture(textTexture, NULL, textSurface->pixels, textSurface->pitch);
	SDL_RenderCopy(renderer, textTexture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	delete currentScreen;
	delete keyboard;
	SDL_DestroyTexture(textTexture);
	SDL_FreeSurface(textSurface);
	SDL_FreeSurface(charset);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	printf("Quiting game...");
}

void Game::changeScreen(SCREEN screenId) {
	printf_s("Change screen to %d\n", screenId);
	currentScreen->~Screen();
	currentScreen = nullptr;
	switch (screenId) {
	case QUIT:
		isRunning = false;
		break;
	case MAIN_MENU:
		currentScreen = new MainMenu(renderer, textSurface, charset, this);
		break;
	case LEVEL_1:
	case LEVEL_2:
	case LEVEL_3:
		currentScreen = new GameScreen(renderer, textSurface, charset, this, screenId, score);
		break;
	}
}

bool Game::running() {
	return isRunning;
}

SDL_Renderer* Game::getRenderer() {
	return renderer;
}

SDL_Texture* Game::loadTextureFromBMP(const char* filepath) {
	SDL_Surface* tmpSurface = SDL_LoadBMP(filepath);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	return texture;
}

KeyboardHandler* Game::getKeyboard() {
	return keyboard;
}
