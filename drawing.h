#pragma once
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"

void drawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset);
void drawTexture(SDL_Renderer* renderer, SDL_Texture* tex, int x, int y);
void drawTextureRotated(SDL_Renderer* renderer, SDL_Texture* tex, int x, int y, double angle = 0.0, float scale = 1.0);
void drawRectangle(SDL_Renderer* renderer, SDL_Rect rect, Color outlineColor, Color fillColor=Color(), bool centered=false);
SDL_Texture* loadTextureFromBMP(SDL_Renderer* renderer, const char* filepath);