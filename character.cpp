#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include <stdio.h>
#include <cstdlib>
#include "character.h"
#include "drawing.h"

#define PI 3.14159265


const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 640;


Player::Player(SDL_Renderer* _renderer, SDL_Texture* _texture,
	SDL_Rect* _camera, KeyboardHandler* _keyboard,
	GameEntities* _entities, SDL_Texture* _collisionTexture)
	: DestroyableEntity{ _renderer, _texture, _camera },
	keyboard{ _keyboard }, entities{ _entities },
	collisionTexture{ _collisionTexture } {
	entityType = PLAYER;
	invincibleTimer = new Timer(INVINCIBLE_TIME);
	attackTimer = new Timer(MAX_ATTACK_TIME, true);
	attackCountdown = new Timer(MAX_ATTACK_TIME);
	attackCountdown->elapsedTime = MAX_ATTACK_TIME;
}

Player::~Player() {
	printf_s("Deleting player\n");
	delete invincibleTimer;
	delete attackTimer;
	delete attackCountdown;
}

void Player::handleEvent(SDL_Event& event) {
	if (keyboard->isPressed(SDLK_UP)) speed += ACCEL;
	if (keyboard->isPressed(SDLK_DOWN)) speed -= ACCEL;
	if (keyboard->isPressed(SDLK_LEFT)) angle -= ROT_SPEED;
	if (keyboard->isPressed(SDLK_RIGHT)) angle += ROT_SPEED;
	if (keyboard->isPressed(SDLK_SPACE)) {
		attack();
	}
	else {
		attacking = false;
		attackTimer->stop();
	}
	if (speed > MAX_SPEED)
		speed = MAX_SPEED;
	else if (speed < 0)
		speed = 0;
}

void Player::attack() {
	if (attackCountdown->elapsedTime >= MAX_ATTACK_TIME &&
		attackTimer->elapsedTime < MAX_ATTACK_TIME) {
		attackTimer->resume();
		angle += ROT_SPEED * ATTACK_ROT_MULTIPLIER;
		attacking = true;
	}
	else {
		attacking = false;
		attackCountdown->start();
	}
}

void Player::update(double delta) {
	Vector2 velocity(cos(angle * PI / 180), sin(angle * PI / 180));
	velocity = velocity.normalized();
	pos.x += velocity.x * delta * speed;
	pos.y += velocity.y * delta * speed;
	attackTimer->update(delta);
	attackCountdown->update(delta);
	//printf("%f\t%f\n", attackTimer->elapsedTime, attackCountdown->elapsedTime);
	if (invincible) {
		if (invincibleTimer->update(delta)) {
			stopInvincibility();
		}
	}
}

void Player::render() {
	DestroyableEntity::render();

	// render energy bar
	int MARGIN = 2;
	Color bgColor = Color(191, 179, 0);
	Color energyColor = Color(255, 247, 0);
	SDL_Rect barRect = SDL_Rect();
	barRect.w = WIDTH;
	barRect.h = WIDTH / 10;
	barRect.x = getGlobalPos().x - WIDTH / 2;
	barRect.y = getGlobalPos().y - WIDTH / 2 - barRect.h;
	drawRectangle(renderer, barRect, bgColor, bgColor);
	barRect.w = barRect.w / (float)MAX_ATTACK_TIME * (MAX_ATTACK_TIME - (int)attackTimer->elapsedTime) - MARGIN * 2;
	barRect.h -= MARGIN * 2;
	barRect.x += MARGIN;
	barRect.y += MARGIN;
	drawRectangle(renderer, barRect, energyColor, energyColor);
}

void Player::collide(Entity* collidingEntity, double delta) {
	colliding = true;
	if (collidingEntity->entityType == WEAPON) {
		Weapon* weapon = dynamic_cast<Weapon*>(collidingEntity);
		if (attacking) {
			weapon->setAngle(weapon->getAngle() + 180); // bounce bullet
			weapon->update(0.05); // prevent infinite loop inside player rect
		}
		else {
			if (!invincible) {
				healthPoints -= weapon->DAMAGE;
				startInvincibility();
			}
			entities->queueRemove(weapon);
		}
	}
	else if (collidingEntity->entityType == ENEMY) {
		Enemy* enemy = dynamic_cast<Enemy*>(collidingEntity);
		if (attacking) {
			enemy->hit(damage * delta);
			Spark* spark = new Spark(renderer, collisionTexture, camera, entities, enemy->getPos());
			entities->addEntity(spark);
		}
	}
}

void Player::startInvincibility() {
	invincibleTimer->start();
	invincible = true;
	SDL_SetTextureColorMod(texture, 255, 103, 129);
}

void Player::stopInvincibility() {
	invincible = false;
	SDL_SetTextureColorMod(texture, 255, 255, 255);
}


Chemiczny::Chemiczny(SDL_Renderer* _renderer, SDL_Texture* _texture,
	SDL_Rect* _camera, GameEntities* _entities, Player* _player, SDL_Texture* _acidTxt)
	: Enemy{ _renderer, _texture, _camera, _entities, _player, _acidTxt } {
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
	if (shootingTimer <= 0 && pos.getDistanceTo(player->getPos()) <= shootingThreshold) {
		Weapon* acidWeapon = new Weapon(renderer, weaponTexture, camera, angle);
		acidWeapon->setPos(pos);
		entities->addEntity(acidWeapon);
		shootingTimer = shootingDelay;
	}
	shootingTimer -= delta * 1000;
}

void Chemiczny::update(double delta) {
	updatePosition(delta);
	tryToShoot(delta);
}