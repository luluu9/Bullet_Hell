#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "cstdlib"
#include "weapons.h"
#include "settings.h"
#include "drawing.h"

#define PI 3.14159265

Weapon::Weapon(
	SDL_Renderer* _renderer,
	char* texturePath,
	Camera* _camera,
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
	Camera* _camera,
	int _angle)
	:Weapon(_renderer, texturePath, _camera, _angle) {
	weaponType = ACID;
}


Robot::Robot(
	SDL_Renderer* _renderer,
	char* texturePath,
	Camera* _camera,
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
	Camera* _camera,
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
	AnimationPlayer* boomAnimation = new AnimationPlayer(renderer, camera, EMP_GRANADE_DIR_PATH, EMP_GRANADE_FRAMES, pos, entities);
	entities->addEntity(boomAnimation);
	boomAnimation->start();
}

EMPWave::EMPWave(
	SDL_Renderer* _renderer,
	char* texturePath,
	Camera* _camera,
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


Hammer::Hammer(
	SDL_Renderer* _renderer,
	char* texturePath,
	Camera* _camera,
	int _angle)
	:Weapon(_renderer, texturePath, _camera, _angle) {
	DAMAGE = _DAMAGE;
	weaponType = HAMMER;
	startAngle = _angle;
}


void Hammer::update(double delta) {
	angle -= delta * 800;
	Vector2 rotationDirection = getDirectionFromAngle(angle);
	pos.x += rotationDirection.x * delta * SPEED;
	pos.y += rotationDirection.y * delta * SPEED;
	Vector2 baseDirection = getDirectionFromAngle(startAngle);
	pos.x += baseDirection.x * delta * SPEED;
	pos.y += baseDirection.y * delta * SPEED;
}



WreckingBall::WreckingBall(
	SDL_Renderer* _renderer,
	char* texturePath,
	Camera* _camera,
	Vector2 _impactPos)
	:Weapon(_renderer, texturePath, _camera, 0) {
	impactPos = _impactPos;
	DAMAGE = _DAMAGE;
	weaponType = HAMMER;
	shadowTexture = loadTextureFromBMP(renderer, WRECKING_BALL_SHADOW_TXT_PATH);
	COLLISIONS_DISABLED = true;
	fallTimer.start();
}


void WreckingBall::render() {
	float shadowScale = fallTimer.elapsedTime / SHOW_BALL_TIME;
	shadowScale = shadowScale < 1.0 ? shadowScale : 1.0;
	SDL_SetTextureAlphaMod(shadowTexture, shadowScale * 255);
	DrawTextureRotated(renderer, shadowTexture, impactPos.x - camera->x, impactPos.y - camera->y, 0, shadowScale);
	if (ballShown) {
		float distFromDest = SCREEN_HEIGHT * FALL_SPEED - fallTimer.elapsedTime / (FALL_TIME)*SCREEN_HEIGHT * FALL_SPEED;
		pos = Vector2(impactPos.x, impactPos.y - distFromDest);
		DrawTextureRotated(renderer, texture, pos.x - camera->x, pos.y - camera->y, 0);
	}
	renderCollisionBox();
}

void WreckingBall::update(double delta) {
	if (fallTimer.update(delta)) {
		COLLISIONS_DISABLED = false;
		shakeTimer.start();
	}
	if (fallTimer.elapsedTime >= SHOW_BALL_TIME) {
		ballShown = true;
	}
	if (COLLISIONS_DISABLED == false && shakeTimer.elapsedTime < SHAKE_DURATION) {
		shakeTimer.update(delta);
		shakeCamera();
	}
}

void WreckingBall::shakeCamera() {
	camera->x += -SHAKE_POWER / 2 + rand() % SHAKE_POWER;
	camera->y += -SHAKE_POWER / 2 + rand() % SHAKE_POWER;
}


Gas::Gas(
	SDL_Renderer* _renderer,
	char* texturePath,
	Camera* _camera,
	GameEntities* _gameEntities,
	Vector2 startPos,
	float _destroyTime,
	float _TRANS_TIME)
	:Weapon(_renderer, texturePath, _camera, 0) {
	gameEntities = _gameEntities;
	DAMAGE = _DAMAGE;
	DESTROY_TIME = _destroyTime;
	TRANS_TIME = _TRANS_TIME != 0 ? _TRANS_TIME : TRANS_TIME;
	weaponType = GAS;
	angle = rand() % 360;
	ROTATION = -ROTATION_MAX + rand() % ROTATION_MAX *2;
	pos = startPos;
	COLLISIONS_DISABLED = true;
}


void Gas::update(double delta) {
	elapsedTime += delta * 1000;
	if (elapsedTime > DESTROY_TIME) {
		alpha = MAX_ALPHA * (1 - ((elapsedTime - DESTROY_TIME) / TRANS_TIME));
		if (alpha <= 0)
			gameEntities->queueRemove(this);
	}
	else {
		alpha = MAX_ALPHA * (elapsedTime / TRANS_TIME);
		scale = MAX_SCALE * (elapsedTime / TRANS_TIME);
		alpha = alpha > MAX_ALPHA ? MAX_ALPHA : alpha;
		scale = scale > MAX_SCALE ? MAX_SCALE : scale;
		if (elapsedTime > TRANS_TIME/2)
			COLLISIONS_DISABLED = false;
	}
}

void Gas::render() {
	angle = ROTATION * elapsedTime / 1000;
	SDL_SetTextureAlphaMod(texture, alpha);
	DrawTextureRotated(renderer, texture, pos.x - camera->x, pos.y - camera->y, angle, scale);
	renderCollisionBox();
}
