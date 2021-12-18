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

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 640;


Entity::Entity(SDL_Renderer* _renderer, SDL_Texture* _texture) {
	pos.x = pos.y = 0;
	speed = angle = 0;
	texture = _texture;
	renderer = _renderer;
	SDL_QueryTexture(texture, NULL, NULL, &WIDTH, &HEIGHT);
}

void Entity::handleEvent(SDL_Event& event) {
	//printf("entity event");
}

void Entity::update(double delta) {
	//printf("entity update");
}

void Entity::render(SDL_Rect cam) {
	//printf_s("%f, %f: %d, %d\n", pos.x, pos.y, cam.x, cam.y);
	DrawTextureRotated(renderer, texture, pos.x - cam.x, pos.y - cam.y, angle);
}

Vector2 Entity::getPos() {
	return pos;
}


Player::Player(SDL_Renderer* _renderer, SDL_Texture* _texture, KeyboardHandler* _keyboard)
	: Entity{ _renderer, _texture }, keyboard{ _keyboard } {}

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
}


Enemy::Enemy(SDL_Renderer* _renderer, SDL_Texture* _texture)
	:Entity(_renderer, _texture) {
	angle = rand() % 360;
}

void Enemy::update(double delta) {
	Vector2 velocity(cos(angle * PI / 180), sin(angle * PI / 180));
	velocity = velocity.normalized();
	pos.x += velocity.x * delta * SPEED;
	pos.y += velocity.y * delta * SPEED;
}