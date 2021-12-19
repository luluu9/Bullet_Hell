#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "character.h"
#include "drawing.h"
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <math.h>

#define PI 3.14159265
#define DRAW_COLLISION_BOX 1

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 640;


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


Player::Player(SDL_Renderer* _renderer, SDL_Texture* _texture, SDL_Rect* _camera, KeyboardHandler* _keyboard)
	: Entity{ _renderer, _texture, _camera }, keyboard{ _keyboard } {}

void Player::handleEvent(SDL_Event& event) {
	//printf("%d", keyboard->isPressed(SDLK_UP));
	if (keyboard->isPressed(SDLK_UP)) speed += ACCEL;
	if (keyboard->isPressed(SDLK_DOWN)) speed -= ACCEL;
	if (keyboard->isPressed(SDLK_LEFT)) angle -= ROT_SPEED;
	if (keyboard->isPressed(SDLK_RIGHT)) angle += ROT_SPEED;
	if (keyboard->isPressed(SDLK_SPACE)) angle += ROT_SPEED * ATTACK_ROT_MULTIPLIER;
	if (speed > MAX_SPEED)
		speed = MAX_SPEED;
	else if (speed < 0)
		speed = 0;
}

void Player::update(double delta) {
	Vector2 velocity(cos(angle * PI / 180), sin(angle * PI / 180));
	velocity = velocity.normalized();
	pos.x += velocity.x * delta * speed;
	pos.y += velocity.y * delta * speed;
	// printf_s("%f, %f: %f\n", velocity.x, velocity.y, speed);
	// print pos
	// printf_s("%d, %d: %d, %d\n", getRect().x, getRect().y, getRect().w, getRect().h);
}

void Player::collide(Entity* collidingEntity) {
	colliding = true;	
}

void Player::collide(Weapon* collidingEntity) {
	colliding = true;
}


Enemy::Enemy(SDL_Renderer* _renderer, SDL_Texture* _texture, SDL_Rect* _camera)
	:Entity(_renderer, _texture, _camera) {
	angle = rand() % 360;
}

void Enemy::update(double delta) {
	Vector2 velocity(cos(angle * PI / 180), sin(angle * PI / 180));
	velocity = velocity.normalized();
	pos.x += velocity.x * delta * SPEED;
	pos.y += velocity.y * delta * SPEED;
}


Weapon::Weapon(SDL_Renderer* _renderer, SDL_Texture* _texture, SDL_Rect* _camera, int _angle)
	:Entity(_renderer, _texture, _camera) {
	angle = _angle;
}

void Weapon::update(double delta) {
	Vector2 velocity(cos(angle * PI / 180), sin(angle * PI / 180));
	velocity = velocity.normalized();
	pos.x += velocity.x * delta * SPEED;
	pos.y += velocity.y * delta * SPEED;
}