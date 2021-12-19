#pragma once
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "base_types.h"


class Entity {
public:
	int WIDTH;
	int HEIGHT;
	bool colliding;

	Entity(SDL_Renderer* _renderer, SDL_Texture* _texture, SDL_Rect* _camera);

	virtual void handleEvent(SDL_Event& event);
	virtual void update(double delta);
	void render();
	Vector2 getPos();
	void setPos(Vector2 newPos);
	SDL_Rect getRect();
	SDL_Rect getGlobalRect();
	void setAngle(int angle);

protected:
	Vector2 pos;
	int speed, angle;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_Rect* camera;
};



class Enemy : public Entity {
public:
	double ACCEL = 30;
	double SPEED = 50;
	double ROT_SPEED = 2;
	double MAX_SPEED = 500; //px per sec

	//Initializes the variablesA
	Enemy(SDL_Renderer* _renderer, SDL_Texture* _texture, SDL_Rect* _camera);

	void update(double delta);
};