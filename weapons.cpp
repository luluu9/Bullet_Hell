#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "weapons.h"

#define PI 3.14159265


Weapon::Weapon(SDL_Renderer* _renderer, SDL_Texture* _texture, SDL_Rect* _camera, int _angle)
	:Entity(_renderer, _texture, _camera) {
	angle = _angle;
	entityType = WEAPON;
}

void Weapon::update(double delta) {
	Vector2 velocity(cos(angle * PI / 180), sin(angle * PI / 180));
	velocity = velocity.normalized();
	pos.x += velocity.x * delta * SPEED;
	pos.y += velocity.y * delta * SPEED;
}