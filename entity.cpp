#include "entity.h"
#include "drawing.h"
#include "settings.h"
#include <cstdlib>

#define DRAW_COLLISION_BOX 1
#define PI 3.14159265


Entity::Entity(
	SDL_Renderer* _renderer, 
	char* texturePath, 
	SDL_Rect* _camera) {
	pos.x = pos.y = 0;
	speed = angle = 0;
	renderer = _renderer;
	texture = loadTextureFromBMP(renderer, texturePath);
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
	char* texturePath,
	SDL_Rect* _camera)
	:Entity(_renderer, texturePath, _camera) { }

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

void DestroyableEntity::setHP(float _MAX_HP) {
	MAX_HP = _MAX_HP;
	healthPoints = MAX_HP;
}

Enemy::Enemy(
	SDL_Renderer* _renderer, 
	char* texturePath,
	SDL_Rect* _camera, 
	GameEntities* _entities,
	Player* _player)
	:DestroyableEntity(_renderer, texturePath, _camera) {
	angle = rand() % 360;
	entities = _entities;
	entityType = ENEMY;
	player = _player;
}

void Enemy::hit(float damage) {
	healthPoints -= damage;
	if (healthPoints <= 0)
		entities->queueRemove(this);
}


GameEntities::GameEntities() {
	for (unsigned int i = 0; i < amount; i++)
		entities[i] = nullptr;
	for (unsigned int i = 0; i < queueAmount; i++)
		removeQueue[i] = nullptr;
}

GameEntities::~GameEntities() {
	printf_s("Deleting game entities\n");
	for (unsigned int i = 0; i < amount; i++)
		delete entities[i];
	delete[] entities;
	delete[] removeQueue;
}

void GameEntities::removeQueuedEntities() {
	// remove all entities queued to remove
	for (unsigned int i = 0; i < currentQueueEntity; i++) {
		removeEntity(removeQueue[i]);
		removeQueue[i] = nullptr;
	}
	currentQueueEntity = 0;
}

Entity* GameEntities::getEntity(unsigned int entityId) {
	if (entityId >= amount) return nullptr;
	return entities[entityId];
}

void GameEntities::addEntity(Entity* entity) {
	if (currentEntity >= amount) {
		// resize array
		amount *= 2;
		Entity** extendedArray = new Entity * [amount];
		for (unsigned int i = 0; i < currentEntity; i++)
			extendedArray[i] = entities[i];
		for (unsigned int i = currentEntity; i < amount; i++)
			extendedArray[i] = nullptr;
		delete[] entities;
		entities = extendedArray;
	}
	entities[currentEntity] = entity;
	currentEntity++;
}

void GameEntities::queueRemove(Entity* entityToRemove) {
	if (currentQueueEntity >= queueAmount) {
		// resize array
		queueAmount *= 2;
		Entity** extendedArray = new Entity * [queueAmount];
		for (unsigned int i = 0; i < currentQueueEntity; i++)
			extendedArray[i] = removeQueue[i];
		for (unsigned int i = currentQueueEntity; i < queueAmount; i++)
			extendedArray[i] = nullptr;
		delete[] removeQueue;
		removeQueue = extendedArray;
	}
	removeQueue[currentQueueEntity] = entityToRemove;
	currentQueueEntity++;
}

void GameEntities::removeEntity(Entity* entityToRemove) {
	Entity* firstEncounteredEntity = nullptr;
	unsigned int firstEncounteredEntityId = amount;
	for (int i = amount - 1; i >= 0; i--) {
		if (entities[i] != nullptr) {
			if (entities[i] == entityToRemove) {
				entities[i]->~Entity(); // call destructor
				entities[i] = nullptr;
				if (firstEncounteredEntity != nullptr) { // fill gap in array
					entities[i] = firstEncounteredEntity;
					entities[firstEncounteredEntityId] = nullptr;
				}
				currentEntity--;
				break;
			}
			if (firstEncounteredEntity == nullptr) {
				firstEncounteredEntity = entities[i];
				firstEncounteredEntityId = i;
			}
		}
	}
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
