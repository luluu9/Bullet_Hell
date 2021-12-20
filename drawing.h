#pragma once
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"

void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset);
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);
void DrawTexture(SDL_Renderer* renderer, SDL_Texture* tex, int x, int y);
void DrawTextureRotated(SDL_Renderer* renderer, SDL_Texture* tex, int x, int y, double angle);
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);
void drawRectangle(SDL_Renderer* renderer, SDL_Rect* rect, Color outlineColor, Color fillColor=Color());