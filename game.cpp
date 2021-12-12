#include <stdio.h>
#include "game.h"



Game::Game() {
	isRunning = true;
	window = nullptr;
	renderer = nullptr;
	SCREEN_WIDTH = 0;
	SCREEN_HEIGHT = 0;
	eti = nullptr;
	player = nullptr;
}

Game::~Game() {
	clean();
}

void Game::init(const char* title, int width, int height, bool fullscreen) {
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;

	int flags = 0;
	if (fullscreen)
		flags = SDL_WINDOW_FULLSCREEN;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf_s("SDL_Init error: %s\n", SDL_GetError());
		isRunning = false;
		return;
	}

	int rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, flags, &window, &renderer);
	if (rc != 0) {
		SDL_Quit();
		sprintf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		isRunning = false;
		return;
	};
	
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "Bullet Hell");

	SDL_ShowCursor(SDL_DISABLE);


	eti = loadTextureFromBMP("./eti.bmp");
	player = new Player(renderer, eti);
}

void Game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false;
			break;
		case SDL_KEYUP:
			break;
		case SDL_QUIT:
			isRunning = false;
			break;
		};
		player->handleEvent(event);
	};
}

void Game::update() {
	player->move(1);
}

void Game::render() {
	SDL_RenderClear(renderer);
	player->render();
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	printf("Quiting game...");	
}

SDL_Renderer* Game::getRenderer() {
	return renderer;
}

bool Game::running() {
	return isRunning;
}


SDL_Texture* Game::loadTextureFromBMP(const char* filepath) {
	SDL_Surface* tmpSurface = SDL_LoadBMP(filepath);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	return texture;
}