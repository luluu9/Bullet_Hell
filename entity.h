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
	SDL_Rect getRect();
	SDL_Rect getGlobalRect();

protected:
	Vector2 pos;
	int speed, angle;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_Rect* camera;
};

