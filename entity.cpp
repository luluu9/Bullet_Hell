#include "entity.h"
#include "drawing.h"
#include <cstdlib>

#define DRAW_COLLISION_BOX 1
#define PI 3.14159265


Entity::Entity(SDL_Renderer* _renderer, SDL_Texture* _texture, SDL_Rect* _camera) {
	pos.x = pos.y = 0;
	speed = angle = 0;
	texture = _texture;
	renderer = _renderer;
	camera = _camera;
	colliding = false;
	SDL_QueryTexture(texture, NULL, NULL, &WIDTH, &HEIGHT);
}

void Entity::handleEvent(SDL_Event& event) {
	//printf("entity event");
}

void Entity::update(double delta) {
	//printf("entity update");
}

void Entity::render() {
	//printf_s("%f, %f: %d, %d\n", pos.x, pos.y, cam.x, cam.y);
	DrawTextureRotated(renderer, texture, pos.x - camera->x, pos.y - camera->y, angle);
	if (DRAW_COLLISION_BOX) {
		SDL_Rect rect = getGlobalRect();
		uint8_t r, g, b, a;
		SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
		if (colliding)
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0); // change drawing color to red
		SDL_RenderDrawRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, r, g, b, a); // return to original color

	}
}

Vector2 Entity::getPos() {
	return pos;
}

void Entity::setPos(Vector2 newPos) {
	pos = newPos;
}

SDL_Rect Entity::getRect() {
	SDL_Rect rect;
	rect.x = pos.x - WIDTH / 2;
	rect.y = pos.y - HEIGHT / 2;
	rect.w = WIDTH;
	rect.h = HEIGHT;
	return rect;
}

SDL_Rect Entity::getGlobalRect() {
	SDL_Rect rect = getRect();
	rect.x -= camera->x;
	rect.y -= camera->y;
	return rect;
}

void Entity::setAngle(int _angle) {
	angle = _angle;
}


Enemy::Enemy(SDL_Renderer* _renderer, SDL_Texture* _texture, SDL_Rect* _camera)
	:Entity(_renderer, _texture, _camera) {
	angle = rand() % 360;
}

void Enemy::update(double delta) {
}


