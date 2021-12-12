#pragma once
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"


class Player {
public:
    const int WIDTH = 40;
    const int HEIGHT = 40;
    const int ACCEL = 10;

    //Initializes the variablesA
    Player(SDL_Surface* _screen, SDL_Surface* _sprite);

    void handleEvent(SDL_Event& event);
    void move(float delta);
    void render();

private:
    int posX, posY;
    int velX, velY;
    SDL_Surface* screen;
    SDL_Surface* sprite;
};
