#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include <cstdlib>
#include <stdio.h>
#include "character.h"
#include "drawing.h"

#define PI 3.14159265


const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 640;


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


