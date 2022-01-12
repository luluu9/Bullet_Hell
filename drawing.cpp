#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "base_types.h"
#include "settings.h"


// narysowanie napisu txt na powierzchni screen, zaczynaj¹c od punktu (x, y)
// charset to bitmapa 128x128 zawieraj¹ca znaki
// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void drawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset) {
	SDL_FillRect(screen, NULL, 0x000000);
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = DEST_CHAR_SIZE;
	d.h = DEST_CHAR_SIZE;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitScaled(charset, &s, screen, &d);
		x += DEST_CHAR_SIZE;
		text++;
	};
};


void drawTexture(SDL_Renderer* renderer, SDL_Texture* tex, int x, int y) {
	SDL_Rect dest;
	int width, height;
	SDL_QueryTexture(tex, NULL, NULL, &width, &height);
	dest.x = x - width / 2;
	dest.y = y - height / 2;
	dest.w = width;
	dest.h = height;
	SDL_RenderCopy(renderer, tex, NULL, &dest);
};


void drawTextureRotated(SDL_Renderer* renderer, SDL_Texture* tex, int x, int y, double angle = 0.0, float scale=1.0) {
	SDL_Rect dest;
	int width, height;
	SDL_QueryTexture(tex, NULL, NULL, &width, &height);
	dest.x = x - width*scale / 2;
	dest.y = y - height*scale / 2;
	dest.w = width*scale;
	dest.h = height*scale;
	SDL_RenderCopyEx(renderer, tex, NULL, &dest, angle, NULL, SDL_FLIP_NONE);
};


void drawRectangle(SDL_Renderer* renderer, SDL_Rect rect, Color outlineColor, Color fillColor=Color(), bool centered=false) {
	uint8_t r, g, b, a; // original colors
	if (centered) {
		rect.x -= rect.w / 2;
		rect.y -= rect.h / 2;
	}
	
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(renderer, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
	
	if (fillColor) 
		SDL_RenderFillRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
	SDL_RenderDrawRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, r, g, b, a); // return to original color
}


SDL_Texture* loadTextureFromBMP(SDL_Renderer* renderer, const char* filepath) {
	SDL_Surface* tmpSurface = SDL_LoadBMP(filepath);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	return texture;
}