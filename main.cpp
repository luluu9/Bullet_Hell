#include <stdio.h>
#include "game.h"
#include "drawing.h"
#include "character.h"

#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"


extern const int SCREEN_WIDTH = 620;
extern const int SCREEN_HEIGHT = 480;
bool FULLSCREEN = false;


int main(int argc, char** argv) {
	double frames, t1, t2, delta, worldTime, fpsTimer, fps;

	Game* game = new Game();
	game->init("Bullet Hell", SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN);

	t1 = SDL_GetTicks();

	frames = 0;
	fpsTimer = 0;
	fps = 0;
	worldTime = 0;

	while (game->running()) {
		t2 = SDL_GetTicks();
		delta = (double(t2) - t1) * 0.001;
		t1 = t2;

		worldTime += delta;
		fpsTimer += delta;
		if (fpsTimer > 0.5) {
			fps = frames * 2;
			frames = 0;
			fpsTimer -= 0.5;
		};
		printf_s("czas trwania = %.1lf s  %.0lf klatek / s\n", worldTime, fps);

		game->handleEvents();
		game->update();
		game->render();
		
		frames++;
	};

	game->clean();
	return 0;
};
