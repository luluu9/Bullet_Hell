#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "weapons.h"

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
	int robotId)
	:Weapon(_renderer, texturePath, _camera, 0) {
	pos.x = startPos.x;
	pos.y = startPos.y;
	if (robotId == 0) pos.y -= radius;
	if (robotId == 1) pos.x += radius;
	if (robotId == 2) pos.y += radius;
	if (robotId == 3) pos.x -= radius;
	angle = rotateSpeed * robotId;
	SPEED = 2 * PI * radius / (360 / rotateSpeed);
	weaponType = ROBOT;
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
	float _startAngle)
	:Weapon(_renderer, texturePath, _camera, _startAngle) {
	startAngle = _startAngle;
	weaponType = EMP_GRANADE;
	SPEED = GRANADE_SPEED;
}

void EMP::update(double delta) {
	angle += ROTATE_SPEED * delta;
	Vector2 direction = getDirectionFromAngle(startAngle);
	pos.x += direction.x * delta * SPEED;
	pos.y += direction.y * delta * SPEED;
}
