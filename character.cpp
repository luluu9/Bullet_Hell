#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "character.h"
#include "drawing.h"


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


Player::Player(SDL_Surface* _screen, SDL_Surface* _sprite) {
	posX = posY = 0;
	velX = velY = 0;
	sprite = _sprite;
	screen = _screen;
}


void Player::handleEvent(SDL_Event& event) {
	if (event.type == SDL_KEYDOWN) {}
	switch (event.key.keysym.sym) {
	case SDLK_UP: velY -= ACCEL; break;
	case SDLK_DOWN: velY += ACCEL; break;
	case SDLK_LEFT: velX -= ACCEL; break;
	case SDLK_RIGHT: velX += ACCEL; break;
	}
}


void Player::move(float delta) {
	posX += velX * delta;
	if ((posX < 0) || (posX + WIDTH > SCREEN_WIDTH))
		posX -= velX;

	posY += velY * delta;
	if ((posY < 0) || (posY + HEIGHT > SCREEN_HEIGHT))
		posY -= velY;

}


void Player::render() {
	DrawSurface(screen, sprite, posX, posY);
}