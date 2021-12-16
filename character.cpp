#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "character.h"
#include "drawing.h"
#include <stdio.h>
#include <math.h>

#define PI 3.14159265

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 640;


Player::Player(SDL_Renderer* _renderer, SDL_Texture* _texture, KeyboardHandler* _keyboard) {
	pos.x = pos.y = 0;
	speed = angle = 0;
	texture = _texture;
	renderer = _renderer;
	keyboard = _keyboard;
}


void Player::handleEvent(SDL_Event& event) {
	if (keyboard->isPressed(SDLK_UP)) speed += ACCEL;
	if (keyboard->isPressed(SDLK_DOWN)) speed -= ACCEL;
	if (keyboard->isPressed(SDLK_LEFT)) angle -= ROT_SPEED;
	if (keyboard->isPressed(SDLK_RIGHT)) angle += ROT_SPEED;
	if (speed > MAX_SPEED)
		speed = MAX_SPEED;
	else if (speed < 0)
		speed = 0;
}


void Player::move(float delta) {
	Vector2 velocity(cos(angle * PI / 180), sin(angle * PI / 180));
	velocity = velocity.normalized();
	pos.x += velocity.x * delta * speed;
	pos.y += velocity.y * delta * speed;
	// printf_s("%f, %f: %f\n", velocity.x, velocity.y, speed);
}


void Player::render(SDL_Rect cam) {
	printf_s("%f, %f: %d, %d\n", pos.x, pos.y, cam.x, cam.y);
	DrawTextureRotated(renderer, texture, pos.x - cam.x, pos.y - cam.y, angle);
}

Vector2 Player::getPos() {
	return pos;
}