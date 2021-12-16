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
	keyboard = nullptr;
	background = nullptr;
	player = nullptr;
	camera = SDL_Rect{ 0 };
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
	eti = loadTextureFromBMP("./assets/etispinner_small.bmp");
	background = loadTextureFromBMP("./assets/sky.bmp");
	player = new Player(renderer, eti, keyboard);
	entities.addEntity(player);

	
	SDL_Texture* chemiczny = loadTextureFromBMP("./assets/chemiczny.bmp");
	for (int i = 0; i < 8; i++) {
		Enemy *enemy = new Enemy(renderer, chemiczny);
		entities.addEntity(enemy);
	}
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
		for (int i = 0; i < entities.currentEntity; i++) {
			entities.entities[i]->handleEvent(event);
		}
	};
}

void Game::update(double delta) {
	SDL_Event x;
	for (int i = 0; i < entities.currentEntity; i++) {
		entities.entities[i]->update(delta);
		entities.entities[i]->handleEvent(x);
	}
	camera.x = (int)(player->getPos().x + player->WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = (int)(player->getPos().y + player->HEIGHT / 2) - SCREEN_HEIGHT / 2;
}

void Game::render() {
	SDL_RenderClear(renderer);
	DrawTexture(renderer, background, -camera.x, -camera.y);
	DrawTexture(renderer, background, -camera.x + 1024, -camera.y);
	DrawTexture(renderer, background, -camera.x + 1024 * 2, -camera.y);
	for (int i = 0; i < entities.currentEntity; i++) {
		entities.entities[i]->render(camera);
	}
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