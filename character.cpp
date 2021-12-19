#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include <stdio.h>
#include <cstdlib>
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


Chemiczny::Chemiczny(SDL_Renderer* _renderer, SDL_Texture* _texture,
	SDL_Rect* _camera, GameEntities* _entities, Player* _player, SDL_Texture* _acidTxt)
	: Enemy{ _renderer, _texture, _camera, _entities },
	player{ _player } { 
	acidTexture = _acidTxt;
	shootingTimer = rand() % shootingDelay;
};


void Chemiczny::updatePosition(double delta) {
	Vector2 playerPos = player->getPos();
	angle = pos.getAngleTo(playerPos);
	Vector2 velocity(cos(angle * PI / 180), sin(angle * PI / 180));
	int threshold = player->WIDTH;
	if (pos.getDistanceTo(playerPos) < threshold) return;
	velocity = velocity.normalized();
	pos.x += velocity.x * delta * SPEED;
	pos.y += velocity.y * delta * SPEED;
}

void Chemiczny::tryToShoot(double delta) {
	if (shootingTimer <= 0){ 
		Weapon* acidWeapon = new Weapon(renderer, acidTexture, camera, angle);
		acidWeapon->setPos(pos);
		entities->addEntity(acidWeapon);
		shootingTimer = shootingDelay;
	}
	shootingTimer -= delta * 1000;
}

void Chemiczny::update (double delta) {
	updatePosition(delta);
	tryToShoot(delta);
}