#include <stdio.h>
#include <cstdlib>
#include "game.h"
#include "drawing.h"
#include "entity.h"
#include "string.h"


Game::Game() {
	isRunning = true;
	window = nullptr;
	renderer = nullptr;
	SCREEN_WIDTH = 0;
	SCREEN_HEIGHT = 0;
	textTexture = nullptr;
	textSurface = nullptr;
	charset = nullptr;
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
		sprintf_s("SDL_CreateWindowAndRenderer error: %s\n", 39*sizeof(char), SDL_GetError());
		isRunning = false;
		return;
	};

	textSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	textTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetTextureBlendMode(textTexture, SDL_BLENDMODE_BLEND);


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

	SDL_Texture* eti = loadTextureFromBMP("./assets/etispinner_small.bmp");
	charset = SDL_LoadBMP("./cs8x8.bmp");
	SDL_SetColorKey(charset, true, 0xFF000000);
	background = loadTextureFromBMP("./assets/sky.bmp");
	player = new Player(renderer, eti, &camera, keyboard, &entities);
	entities.addEntity(player);
	
	SDL_Texture* chemiczny = loadTextureFromBMP("./assets/chemiczny.bmp");
	SDL_Texture* acid = loadTextureFromBMP("./assets/acid.bmp");
	for (int i = 0; i < 5; i++) {
		Chemiczny *enemy = new Chemiczny(renderer, chemiczny, &camera, &entities, player, acid);
		entities.addEntity(enemy);
		enemy->setPos(Vector2(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT));
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
		for (unsigned int i = 0; i < entities.currentEntity; i++) {
			entities.getEntity(i)->handleEvent(event);
		}
	};
}

bool isColliding(Entity* a, Entity* b) {
	// AABB algorithm
	SDL_Rect rect1 = a->getGlobalRect();
	SDL_Rect rect2 = b->getGlobalRect();
	if (rect1.x < rect2.x + rect2.w &&
		rect1.x + rect1.w > rect2.x &&
		rect1.y < rect2.y + rect2.h &&
		rect1.h + rect1.y > rect2.y) {
		return true;
	}
	return false;
}

void Game::update(double delta, double worldTime) {
	entities.removeQueuedEntities();
	SDL_Event x;
	for (unsigned int i = 0; i < entities.currentEntity; i++) {
		Entity* currentEntity = entities.getEntity(i);
		currentEntity->handleEvent(x);
		currentEntity->update(delta);
	}
	camera.x = (int)(player->getPos().x + player->WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = (int)(player->getPos().y + player->HEIGHT / 2) - SCREEN_HEIGHT / 2;

	player->colliding = false;
	for (unsigned int i = 0; i < entities.currentEntity; i++) {
		Entity* currentEntity = entities.getEntity(i);	
		if (currentEntity != player) {
			if (isColliding(player, currentEntity)) {
				player->collide(currentEntity, delta);
			}	
		}
	}

	sprintf_s(text, "% .1lf s ", worldTime);
	DrawString(textSurface, SCREEN_WIDTH / 2 , 10, text, charset);
}

void Game::render() {
	SDL_RenderClear(renderer);
	DrawTexture(renderer, background, -camera.x, -camera.y);
	DrawTexture(renderer, background, -camera.x + 1024, -camera.y);
	DrawTexture(renderer, background, -camera.x + 1024 * 2, -camera.y);
	for (unsigned int i = 0; i < entities.currentEntity; i++)
		entities.getEntity(i)->render();
	SDL_UpdateTexture(textTexture, NULL, textSurface->pixels, textSurface->pitch);
	SDL_RenderCopy(renderer, textTexture, NULL, NULL);
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