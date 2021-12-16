#include <stdio.h>
#include "game.h"
#include "drawing.h"


Game::Game() {
	isRunning = true;
	window = nullptr;
	renderer = nullptr;
	SCREEN_WIDTH = 0;
	SCREEN_HEIGHT = 0;
	eti = nullptr;
	player = nullptr;
	keyboard = nullptr;
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

	keyboard = new KeyboardHandler;

	camera.x = 0;
	camera.y = 0;
	camera.w = SCREEN_WIDTH;
	camera.h = SCREEN_HEIGHT;
	eti = loadTextureFromBMP("./eti.bmp");
	background = loadTextureFromBMP("./assets/sky.bmp");
	player = new Player(renderer, eti, keyboard);
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
		keyboard->handleEvent(event);
		player->handleEvent(event);
	};
}

void Game::update(double delta) {
	SDL_Event x;
	player->handleEvent(x);
	player->update(delta);
	camera.x = (int)(player->getPos().x + player->WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = (int)(player->getPos().y + player->HEIGHT / 2) - SCREEN_HEIGHT / 2;
}

void Game::render() {
	SDL_RenderClear(renderer);
	DrawTexture(renderer, background, -camera.x, -camera.y);
	DrawTexture(renderer, background, -camera.x + 1024, -camera.y);
	DrawTexture(renderer, background, -camera.x + 1024 * 2, -camera.y);
	player->render(camera);
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