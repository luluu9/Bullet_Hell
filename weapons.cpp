#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "weapons.h"
#include "settings.h"
#include "drawing.h"

#define PI 3.14159265


Weapon::Weapon(
	SDL_Renderer* _renderer,
	char* texturePath,
	SDL_Rect* _camera,
	int _angle)
	:Entity(_renderer, texturePath, _camera) {
	angle = _angle;
	entityType = WEAPON;
}

void Weapon::update(double delta) {
	Vector2 velocity = getDirectionFromAngle(angle);
	pos.x += velocity.x * delta * SPEED;
	pos.y += velocity.y * delta * SPEED;
}


Acid::Acid(
	SDL_Renderer* _renderer,
	char* texturePath,
	SDL_Rect* _camera,
	int _angle)
	:Weapon(_renderer, texturePath, _camera, _angle) {
	weaponType = ACID;
}


Robot::Robot(
	SDL_Renderer* _renderer,
	char* texturePath,
	SDL_Rect* _camera,
	Vector2 startPos,
	int startAngle,
	float _radius,
	int _rotateSpeed)
	: Weapon(_renderer, texturePath, _camera, 0) {
	weaponType = ROBOT;
	pos = startPos;
	angle = startAngle;
	radius = _radius;
	rotateSpeed = _rotateSpeed;
	SPEED = 2 * PI * radius / (360 / rotateSpeed);
}

void Robot::update(double delta) {
	angle += delta * rotateSpeed;
	Vector2 velocity = getDirectionFromAngle(angle);
	pos.x += velocity.x * delta * SPEED;
	pos.y += velocity.y * delta * SPEED;
}


EMP::EMP(
	SDL_Renderer* _renderer,
	char* texturePath,
	SDL_Rect* _camera,
	GameEntities* _entities,
	float _startAngle)
	:Weapon(_renderer, texturePath, _camera, _startAngle) {
	startAngle = _startAngle;
	weaponType = EMP_GRANADE;
	SPEED = GRANADE_SPEED;
	entities = _entities;
	boomTimer.start();
}

void EMP::update(double delta) {
	if (boomTimer.update(delta)) {
		// boom
		startAnimation();
		int angleStep = 360 / BOOM_DIRECTIONS;
		for (int i = 0; i < BOOM_DIRECTIONS; i++) {
			int waveAngle = startAngle + i * angleStep;
			EMPWave* wave = new EMPWave(renderer, EMPWave_TXT_PATH, camera, entities, waveAngle);
			Vector2 startPos = getPos();
			Vector2 direction = getDirectionFromAngle(waveAngle);
			wave->setPos(startPos);
			entities->addEntity(wave);
		}
		entities->queueRemove(this);
	}
	else {
		angle += ROTATE_SPEED * delta;
		Vector2 direction = getDirectionFromAngle(startAngle);
		pos.x += direction.x * delta * SPEED;
		pos.y += direction.y * delta * SPEED;
	}
}

void EMP::render() {
	Weapon::render();
}

void EMP::startAnimation() {
	AnimationPlayer* boomAnimation = new AnimationPlayer(renderer, camera, EMP_GRANADE_DIR_PATH, EMP_GRANADE_FRAMES, pos);
	entities->addEntity(boomAnimation);
	boomAnimation->start();
}

EMPWave::EMPWave(
	SDL_Renderer* _renderer,
	char* texturePath,
	SDL_Rect* _camera,
	GameEntities* _entities,
	float _startAngle)
	:Weapon(_renderer, texturePath, _camera, _startAngle) {
	entities = _entities;
}

void EMPWave::update(double delta) {
	Vector2 direction = getDirectionFromAngle(angle);
	pos.x += direction.x * delta * SPEED;
	pos.y += direction.y * delta * SPEED;
	scale += delta;
	if (scale > 1.0) {
		entities->queueRemove(this);
	}
}

void EMPWave::render() {
	DrawTextureRotated(renderer, texture, pos.x - camera->x, pos.y - camera->y, angle, scale);
	
}

