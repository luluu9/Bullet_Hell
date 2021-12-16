#pragma once
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include <math.h>
#include "keyboard.h"


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


class Entity {
public:
	int WIDTH;
	int HEIGHT;

	Entity(SDL_Renderer* _renderer, SDL_Texture* _texture);

	virtual void handleEvent(SDL_Event& event);
	virtual void update(double delta);
	void render(SDL_Rect camera);
	Vector2 getPos();

protected:
	Vector2 pos;
	int speed, angle;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
};

class Player : public Entity {
public:
	int WIDTH = 40;
	int HEIGHT = 40;
	double ACCEL = 30;
	double ROT_SPEED = 2;
	double MAX_SPEED = 500; //px per sec
	int ATTACK_ROT_MULTIPLIER = 10;

	//Initializes the variablesA
	Player(SDL_Renderer* _renderer, SDL_Texture* _texture, KeyboardHandler* _keyboard);

	void handleEvent(SDL_Event& event);
	void update(double delta);

private:
	KeyboardHandler* keyboard;
};



class Enemy : public Entity {
public:
	int WIDTH = 40;
	int HEIGHT = 40;
	double ACCEL = 30;
	double SPEED = 50;
	double ROT_SPEED = 2;
	double MAX_SPEED = 500; //px per sec

	//Initializes the variablesA
	Enemy(SDL_Renderer* _renderer, SDL_Texture* _texture);

	void update(double delta);
};
