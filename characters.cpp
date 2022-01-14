#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include <stdio.h>
#include <cstdlib>
#include "characters.h"
#include "drawing.h"
#include "settings.h"


Player::Player(
	SDL_Renderer* _renderer,
	Camera* _camera,
	KeyboardHandler* _keyboard,
	GameEntities* _entities,
	ScoreCounter* _score)
	: DestroyableEntity{ _renderer, PLAYER_TXT_PATH, _camera },
	keyboard{ _keyboard },
	entities{ _entities },
	score{ _score } {
	entityType = PLAYER;
	invincibleTimer = new Timer(INVINCIBLE_TIME);
	attackTimer = new Timer(MAX_ATTACK_TIME, true);
	attackCountdown = new Timer(MAX_ATTACK_TIME);
	attackCountdown->elapsedTime = MAX_ATTACK_TIME;
	camera->x = pos.x - SCREEN_WIDTH / 2;
	camera->y = (pos.y + HEIGHT / 2) - SCREEN_HEIGHT / 2;
	setHP(PLAYER_HP);
	addBonus();
	addBonus();
	addBonus();
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
	Vector2 direction = getDirectionFromAngle(angle);
	Vector2 velocity = direction * delta * speed;
	pos.x += velocity.x;
	pos.y += velocity.y;
	camera->x += velocity.x;
	camera->y += velocity.y;
	camera->setPos(pos - Vector2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
	camera->update(delta);
	attackTimer->update(delta);
	attackCountdown->update(delta);
	gassed = false;
	if (invincible) {
		if (invincibleTimer->update(delta)) {
			stopInvincibility();
		}
	}
}

void Player::renderEnergy() {
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

void Player::render() {
	DestroyableEntity::render();
	renderEnergy();
}

void Player::collide(Entity* collidingEntity, double delta) {
	colliding = true;
	if (collidingEntity->entityType == WEAPON) {
		Weapon* weapon = dynamic_cast<Weapon*>(collidingEntity);
		collideWeapon(weapon, delta);
	}
	else if (collidingEntity->entityType == ENEMY) {
		Enemy* enemy = dynamic_cast<Enemy*>(collidingEntity);
		collideEnemy(enemy, delta);
	}
	else if (collidingEntity->entityType == BONUS) {
		healthPoints += BONUS_HP;
		healthPoints = healthPoints > MAX_HP ? MAX_HP : healthPoints;
		entities->queueRemove(collidingEntity);
	}
}

void Player::collideEnemy(Enemy* enemy, double delta) {
	if (attacking) {
		enemy->hit(damage * delta);
		Spark* spark = new Spark(renderer, camera, entities, enemy->getPos());
		entities->addEntity(spark);
		score->addScore(ATTACK_SCORE);
	}
}

void Player::collideWeapon(Weapon* weapon, double delta) {
	if (attacking && weapon->weaponType == ACID) {
		weapon->setAngle(weapon->getAngle() + 180); // bounce bullet
		weapon->update(0.05); // prevent infinite loop inside player rect
	}
	else if (weapon->weaponType == GAS) {
		if (!gassed) {
			Gas* gas = dynamic_cast<Gas*>(weapon);
			healthPoints -= gas->DAMAGE * delta;
			gassed = true;
		}
	}
	else {
		if (!invincible) {
			healthPoints -= weapon->DAMAGE;
			healthPoints = (healthPoints < 0.0) ? 0.0 : healthPoints;
			startInvincibility();
			camera->startShake(HIT_SHAKE_DURATION, HIT_SHAKE);
		}
		entities->queueRemove(weapon);
		score->removeScore(DMG_SCORE);
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

void Player::addBonus() {
	Bonus* bonusPoint = new Bonus(renderer, camera);
	entities->addEntity(bonusPoint);
	Vector2 bonusPos = Vector2(
		-BONUS_MARGIN / 2 + rand() % BONUS_MARGIN,
		-BONUS_MARGIN / 2 + rand() % BONUS_MARGIN);
	// make bonus to be outside screen
	if (bonusPos.x < 0) bonusPos.x -= SCREEN_WIDTH/2;
	else bonusPos.x += SCREEN_WIDTH;
	if (bonusPos.y < 0) bonusPos.y -= SCREEN_HEIGHT/2;
	else bonusPos.y += SCREEN_HEIGHT;
	bonusPoint->setPos(bonusPos);
	printf_s("Bonus pos: %f, %f\n", bonusPos.x, bonusPos.y);
}

Spark::Spark(
	SDL_Renderer* _renderer,
	Camera* _camera,
	GameEntities* _entities,
	Vector2 startPos)
	:Entity(_renderer, SPARK_TXT_PATH, _camera) {
	pos = startPos;
	pos.x += -MAX_RAND_POS + rand() % (MAX_RAND_POS * 2);
	pos.y += -MAX_RAND_POS + rand() % (MAX_RAND_POS * 2);
	entities = _entities;
	speed = MIN_SPEED + rand() % (MAX_SPEED - MIN_SPEED);
	angle = rand() % MAX_ANGLE;
	scale = (float)rand() / (float)RAND_MAX;
}

void Spark::update(double delta) {
	Vector2 velocity = getDirectionFromAngle(angle);
	pos.x += velocity.x * delta * speed;
	pos.y += velocity.y * delta * speed;
	scale -= SCALE_DECREASE;
	if (scale <= 0)
		entities->queueRemove(this);
}

void Spark::render() {
	drawTextureRotated(renderer, texture, pos.x - camera->x, pos.y - camera->y, angle, scale);
}


Chemiczny::Chemiczny(
	SDL_Renderer* _renderer,
	Camera* _camera,
	GameEntities* _entities,
	Player* _player)
	: Enemy{ _renderer, CHEMICZNY_TXT_PATH, _camera, _entities, _player } {
	enemyType = CHEMICZNY;
	setHP(CHEMICZNY_HP);
	shootingTimer = new Timer(SHOOTING_DELAY, false, true);
	shootingTimer->elapsedTime = rand() % SHOOTING_DELAY;
	gasTimer = new Timer(GAS_DELAY, false, true);

	SDL_Texture* gasTxt = loadTextureFromBMP(renderer, GAS_TXT_PATH);
	SDL_QueryTexture(texture, NULL, NULL, &GAS_WEAPON_WIDTH, &GAS_WEAPON_HEIGHT);
	SDL_DestroyTexture(gasTxt);
	int gasNumberToFill = GAS_WIDTH / GAS_WEAPON_WIDTH;
	addGasTime = GAS_SPEED / gasNumberToFill;
	addGasTimer = new Timer(addGasTime);
	GAS_WEAPON_HEIGHT /= 2;
	gasNumberToFillHeight = GAS_HEIGHT / GAS_WEAPON_HEIGHT;

	shootingTimer->start();
	gasTimer->start();
};

void Chemiczny::updatePosition(double delta) {
	Vector2 playerPos = player->getPos();
	angle = pos.getAngleTo(playerPos);
	Vector2 velocity = getDirectionFromAngle(angle);
	if (pos.getDistanceTo(playerPos) < SHOOTING_THRESHOLD) return;
	pos.x += velocity.x * delta * SPEED;
	pos.y += velocity.y * delta * SPEED;
}

void Chemiczny::gasOut() {
	gasStartPos = player->getPos();
	gasStartPos.x -= GAS_WIDTH / 4;
	gasStartPos.y -= GAS_HEIGHT / 2;
	gasEjected = 0;
	updateGas();
}

void Chemiczny::tryToShoot(double delta) {
	if (shootingTimer->update(delta) && pos.getDistanceTo(player->getPos()) <= SHOOTING_THRESHOLD) {
		int currentWeaponAngle = 0;
		for (int i = 0; i < SHOOTING_SIDES; i++) {
			Weapon* acidWeapon = new Acid(renderer, ACID_TXT_PATH, camera, currentWeaponAngle);
			acidWeapon->setPos(pos);
			entities->addEntity(acidWeapon);
			currentWeaponAngle += 360 / SHOOTING_SIDES;
		}
	}
	if (gasTimer->update(delta)) {
		gasOut();
	}
	updateGas(delta);
}

void Chemiczny::updateGas(double delta) {
	if (gasEjected < GAS_WIDTH && (delta == 0.0 || addGasTimer->update(delta))) {
		Vector2 gasPos = gasStartPos;
		gasPos.x += gasEjected;
		for (int i = 0; i < gasNumberToFillHeight; i++) {
			gasPos.y += GAS_WEAPON_HEIGHT;
			Gas* gas = new Gas(renderer, GAS_TXT_PATH, camera, entities, gasPos, GAS_DELAY);
			entities->addEntity(gas);
		}
		gasEjected += GAS_WEAPON_WIDTH;
		addGasTimer->start();
	}
}

void Chemiczny::update(double delta) {
	updatePosition(delta);
	tryToShoot(delta);
}


AIR::AIR(
	SDL_Renderer* _renderer,
	Camera* _camera,
	GameEntities* _entities,
	Player* _player)
	: Enemy{ _renderer, AIR_TXT_PATH, _camera, _entities, _player } {
	enemyType = AIR_TYPE;
	setHP(AIR_HP);
	angle = 0;
	robots = new Robot * [ROBOTS_NUMBER];
	int angleStep = 360 / ROBOTS_NUMBER;
	for (int i = 0; i < ROBOTS_NUMBER; i++) {
		int robotAngle = i * angleStep;
		Vector2 startPos = getRobotPos(robotAngle);
		Robot* robot = new Robot(renderer, ROBOT_TXT_PATH, camera, startPos, robotAngle, ROBOTS_RADIUS, angleStep);
		entities->addEntity(robot);
		robots[i] = robot;
	}
	shootingTimer = new Timer(SHOOTING_DELAY, false, true);
	shootingTimer->start();
};

AIR::~AIR() {
	for (int i = 0; i < ROBOTS_NUMBER; i++)
		delete robots[i];
	delete[] robots;
	delete robots;
	delete shootingTimer;
}

Vector2 AIR::getRobotPos(int angle) {
	Vector2 startPos = getPos();
	Vector2 direction = getDirectionFromAngle(angle - 90);
	startPos.x += direction.x * ROBOTS_RADIUS;
	startPos.y += direction.y * ROBOTS_RADIUS;
	return startPos;
}

void AIR::setPos(Vector2 newPos) {
	Entity::setPos(newPos);
	int angleStep = 360 / ROBOTS_NUMBER;
	for (int i = 0; i < ROBOTS_NUMBER; i++) {
		int robotAngle = i * angleStep;
		Vector2 pos = getRobotPos(robotAngle);
		robots[i]->setPos(pos);
		robots[i]->setAngle(robotAngle);
	}
}

void AIR::updatePosition(double delta) {
	//printf_s("%f, %f\n", robots[0]->getPos().x, robots[0]->getPos().y);
	Vector2 playerPos = player->getPos();
	angle = pos.getAngleTo(playerPos);
	Vector2 velocity = getDirectionFromAngle(angle);
	if (pos.getDistanceTo(playerPos) < SHOOTING_THRESHOLD) return;
	pos.x += velocity.x * delta * SPEED;
	pos.y += velocity.y * delta * SPEED;
	for (int i = 0; i < ROBOTS_NUMBER; i++) {
		if (robots[i] == nullptr) continue;
		Vector2 newRobotPos = robots[i]->getPos();
		newRobotPos.x += velocity.x * delta * SPEED;
		newRobotPos.y += velocity.y * delta * SPEED;
		robots[i]->setPos(newRobotPos);
	}
}

void AIR::tryToShoot(double delta) {
	if (shootingTimer->update(delta)) {
		EMP* emp = new EMP(renderer, EMP_TXT_PATH, camera, entities, angle);
		emp->setPos(getPos());
		entities->addEntity(emp);
	}
}


void AIR::update(double delta) {
	updatePosition(delta);
	tryToShoot(delta);
}


WILIS::WILIS(
	SDL_Renderer* _renderer,
	Camera* _camera,
	GameEntities* _entities,
	Player* _player)
	: Enemy{ _renderer, WILIS_TXT_PATH, _camera, _entities, _player } {
	hammerShootingTimer = new Timer(HAMMER_SHOOTING_DELAY, false, true);
	hammerShootingTimer->elapsedTime = rand() % HAMMER_SHOOTING_DELAY;
	hammerShootingTimer->start();
	ballShootingTimer = new Timer(BALL_SHOOTING_DELAY, false, true);
	ballShootingTimer->start();
	ballShootingTimer->elapsedTime = 6000;
	setHP(WILIS_HP);
	enemyType = WILIS_TYPE;
};

void WILIS::updatePosition(double delta) {
	Vector2 playerPos = player->getPos();
	angle = pos.getAngleTo(playerPos);
	Vector2 velocity = getDirectionFromAngle(angle);
	if (pos.getDistanceTo(playerPos) < HAMMER_SHOOTING_THRESHOLD) return;
	pos.x += velocity.x * delta * SPEED;
	pos.y += velocity.y * delta * SPEED;
}

void WILIS::tryToShoot(double delta) {
	if (hammerShootingTimer->update(delta) && pos.getDistanceTo(player->getPos()) <= HAMMER_SHOOTING_THRESHOLD) {
		Weapon* acidWeapon = new Hammer(renderer, HAMMER_TXT_PATH, camera, angle);
		acidWeapon->setPos(pos);
		entities->addEntity(acidWeapon);
	}
	if (ballShootingTimer->update(delta)) {
		Vector2 ballDestination = player->getPos();
		Weapon* ballWeapon = new WreckingBall(renderer, WRECKING_BALL_TXT_PATH, camera, ballDestination);
		entities->addEntity(ballWeapon);
	}
}

void WILIS::update(double delta) {
	updatePosition(delta);
	tryToShoot(delta);
}