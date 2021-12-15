#pragma once
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include <math.h>


struct Vector2 {
	float x;
	float y;

	Vector2() {
		x = 0;
		y = 0;
	}
	Vector2(float _x, float _y) {
		x = _x;
		y = _y;
	};

	Vector2 normalized() {
		float magnitude = sqrtf(x * x + y * y);
		return Vector2(x / magnitude, y / magnitude);
	}
};

class Player {
public:
	const int WIDTH = 40;
	const int HEIGHT = 40;
	const double ACCEL = 30;
	const double ROT_SPEED = 2;
	const double MAX_SPEED = 500; //px per sec

	//Initializes the variablesA
	Player(SDL_Renderer* _renderer, SDL_Texture* _texture);

	void handleEvent(SDL_Event& event);
	void move(float delta);
	void render(SDL_Rect camera);
	Vector2 getPos();

private:
	Vector2 pos;
	int angle;
	float speed;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
};
