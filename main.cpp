#include <stdio.h>
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "game.h"

extern const int SCREEN_WIDTH = 1080;
extern const int SCREEN_HEIGHT = 640;
bool FULLSCREEN = false;
const int MAX_FPS = 144;

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
		game->update(delta);
		game->render();

		
		// limit framerate to MAX_FPS
		float desired_delta = 1.0 / MAX_FPS;
		//printf_s("%f, %f, %d\n", delta, desired_delta, delta < desired_delta);
		if (delta < desired_delta) {
			SDL_Delay((desired_delta-delta)*2000);
		}
		
		frames++;
	};

	game->clean();
	return 0;
};
