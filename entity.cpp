#include "entity.h"
#include "drawing.h"
#include <cstdlib>

#define DRAW_COLLISION_BOX 1
#define PI 3.14159265


Entity::Entity(SDL_Renderer* _renderer, SDL_Texture* _texture,
	SDL_Rect* _camera) {
	pos.x = pos.y = 0;
	speed = angle = 0;
	texture = _texture;
	renderer = _renderer;
	camera = _camera;
	colliding = false;
	entityType = UNKNOWN;
	SDL_QueryTexture(texture, NULL, NULL, &WIDTH, &HEIGHT);
}

Entity::~Entity() {
	printf_s("Deleting entity\n");
	SDL_DestroyTexture(texture);
}

void Entity::handleEvent(SDL_Event& event) {
	//printf("entity event");
}

void Entity::update(double delta) {
	//printf("entity update");
}

void Entity::render() {
	//printf_s("%f, %f: %d, %d\n", pos.x, pos.y, cam.x, cam.y);
	DrawTextureRotated(renderer, texture, pos.x - camera->x, pos.y - camera->y, angle);
	if (DRAW_COLLISION_BOX) {
		SDL_Rect rect = getGlobalRect();
		if (colliding)
			drawRectangle(renderer, rect, Color(255, 0, 0, 0));
		else
			drawRectangle(renderer, rect, Color(0, 0, 0, 0));
	}
}

Vector2 Entity::getPos() {
	return pos;
}

void Entity::setPos(Vector2 newPos) {
	pos = newPos;
}

Vector2 Entity::getGlobalPos() {
	Vector2 globalPos = Vector2(pos.x - camera->x, pos.y - camera->y);
	return globalPos;
}

SDL_Rect Entity::getRect() {
	SDL_Rect rect;
	rect.x = pos.x - WIDTH / 2;
	rect.y = pos.y - HEIGHT / 2;
	rect.w = WIDTH;
	rect.h = HEIGHT;
	return rect;
}

SDL_Rect Entity::getGlobalRect() {
	SDL_Rect rect = getRect();
	rect.x -= camera->x;
	rect.y -= camera->y;
	return rect;
}

int Entity::getAngle() {
	return angle;
}

void Entity::setAngle(int _angle) {
	angle = _angle;
}


DestroyableEntity::DestroyableEntity(
	SDL_Renderer* _renderer,
	SDL_Texture* _texture,
	SDL_Rect* _camera)
	:Entity(_renderer, _texture, _camera) { }

void DestroyableEntity::render() {
	Entity::render();
	drawHPBar();
}

void DestroyableEntity::drawHPBar() {
	int MARGIN = 2;
	Color bgColor = Color(143, 14, 0);
	Color hpColor = Color(207, 21, 0);
	SDL_Rect barRect = SDL_Rect();
	barRect.w = WIDTH;
	barRect.h = WIDTH / 10;
	barRect.x = getGlobalPos().x - WIDTH / 2;
	barRect.y = getGlobalPos().y - WIDTH / 2;
	drawRectangle(renderer, barRect, bgColor, bgColor);
	barRect.w = (barRect.w) / MAX_HP * healthPoints - MARGIN * 2;
	barRect.h -= MARGIN * 2;
	barRect.x += MARGIN;
	barRect.y += MARGIN;
	drawRectangle(renderer, barRect, hpColor, hpColor);
}

Enemy::Enemy(SDL_Renderer* _renderer, SDL_Texture* _texture,
	SDL_Rect* _camera, GameEntities* _entities,
	Player* _player, SDL_Texture* _weaponTexture)
	:DestroyableEntity(_renderer, _texture, _camera) {
	angle = rand() % 360;
	entities = _entities;
	entityType = ENEMY;
	player = _player;
	weaponTexture = _weaponTexture;
}

void Enemy::hit(float damage) {
	healthPoints -= damage;
	if (healthPoints <= 0)
		entities->queueRemove(this);
}


Spark::Spark(SDL_Renderer* _renderer, SDL_Texture* _texture,
	SDL_Rect* _camera, GameEntities* _entities, Vector2 startPos)
	:Entity(_renderer, _texture, _camera) {
	pos = startPos;
	pos.x += -MAX_RAND_POS + rand() % (MAX_RAND_POS * 2);
	pos.y += -MAX_RAND_POS + rand() % (MAX_RAND_POS * 2);
	entities = _entities;
	speed = MIN_SPEED + rand() % (MAX_SPEED - MIN_SPEED);
	angle = rand() % MAX_ANGLE;
	scale = (float)rand() / (float)RAND_MAX;
}

void Spark::update(double delta) {
	Vector2 velocity(cos(angle * PI / 180), sin(angle * PI / 180));
	velocity = velocity.normalized();
	pos.x += velocity.x * delta * speed;
	pos.y += velocity.y * delta * speed;
	scale -= SCALE_DECREASE;
	if (scale <= 0)
		entities->queueRemove(this);
}

void Spark::render() {
	DrawTextureRotated(renderer, texture, pos.x - camera->x, pos.y - camera->y, angle, scale);
}


bool isColliding(Entity* a, Entity* b) {
	// AABB algorithm
	SDL_Rect rect1 = a->getGlobalRect();
	SDL_Rect rect2 = b->getGlobalRect();
	if (rect1.x < rect2.x + rect2.w &&
		rect1.x + rect1.w > rect2.x &&
		rect1.y < rect2.y + rect2.h &&
		rect1.h + rect1.y > rect2.y) {
		return true;
	}
	return false;
}
