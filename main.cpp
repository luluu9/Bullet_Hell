#include <stdio.h>
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "game.h"
#include "settings.h"


int main(int argc, char** argv) {
	int delta;
	double frames, t1, t2, worldTime, fpsTimer, fps, deltaDivided;

	Game* game = new Game();
	t1 = SDL_GetTicks();

	frames = 0;
	fpsTimer = 0;
	fps = 0;
	worldTime = 0;

	while (game->running()) {
		t2 = SDL_GetTicks();
		delta = t2 - t1;
		deltaDivided = (double(t2) - t1) * 0.001;
		t1 = t2;

		worldTime += deltaDivided;
		fpsTimer += deltaDivided;
		if (fpsTimer > 0.5) {
			fps = frames * 2;
			frames = 0;
			fpsTimer -= 0.5;
		};
		// printf_s("czas trwania = %.1lf s  %.0lf klatek / s\n", worldTime, fps);

		game->handleEvents();
		game->update(deltaDivided, worldTime);
		game->render();

		
		// limit framerate to MAX_FPS
		int desired_delta = 1000 / MAX_FPS;
		// printf_s("%f, %f, %d\n", delta, desired_delta, delta < desired_delta);
		if (delta < desired_delta) {
			SDL_Delay(desired_delta-delta);
		}
		
		frames++;
	};

	game->clean();
	return 0;
};
