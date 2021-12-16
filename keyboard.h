#pragma once
#include "./SDL2-2.0.10/include/SDL.h"
const int KEY_CODES_NUMBER = 283;
enum KEY_STATES { RELEASED, PRESSED };

struct KeyboardHandler {
    void handleEvent(SDL_Event event);
    bool isPressed(int keyCode);
    bool isReleased(int keyCode);
    int handleKeyCode(int keyCode);

    bool keyState[KEY_CODES_NUMBER] = { false };
};
