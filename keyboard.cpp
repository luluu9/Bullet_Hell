#pragma once
#include "keyboard.h"


void KeyboardHandler::handleEvent(SDL_Event event) {
    if (event.type == SDL_KEYDOWN) {
        int keyCode = handleKeyCode(event.key.keysym.sym);
        keyState[keyCode] = PRESSED;
    }
    else if (event.type == SDL_KEYUP) {
        int keyCode = handleKeyCode(event.key.keysym.sym);
        keyState[keyCode] = RELEASED;
    }
}

bool KeyboardHandler::isPressed(int keyCode) {
    keyCode = handleKeyCode(keyCode);
    return (keyState[keyCode] == PRESSED);
}

bool KeyboardHandler::isReleased(int keyCode) {
    keyCode = handleKeyCode(keyCode);
    return (keyState[keyCode] == RELEASED);
}

int KeyboardHandler::handleKeyCode(int keyCode) {
    if (keyCode >= SDLK_CAPSLOCK) // https://wiki.libsdl.org/SDLKeycodeLookup
        return SDLK_DELETE + keyCode - SDLK_CAPSLOCK;
    else
        return keyCode;
}