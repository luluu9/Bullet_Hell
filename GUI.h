#pragma once
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "base_types.h"
#include "entity.h"


class GUIElement; // forward declaration

class Screen {
public:
	Screen::Screen(
		SDL_Renderer* _renderer, 
		SDL_Surface* _textSurface, 
		SDL_Surface* _charset,
		int _SCREEN_WIDTH,
		int _SCREEN_HEIGHT);
	void render();
	void handleEvent(SDL_Event& event); 
	void addElement(GUIElement* element);

protected:
	int SCREEN_WIDTH, SCREEN_HEIGHT;
	SDL_Renderer* renderer;
	SDL_Surface* textSurface;
	SDL_Surface* charset;
	GUIElement** elements;
	unsigned int elementsNumber = 10;
	unsigned int elementsCount = 0;
};


class MainMenu : public Screen {
public:
	MainMenu(
		SDL_Renderer* _renderer,
		SDL_Surface* _textSurface,
		SDL_Surface* _charset,
		int _SCREEN_WIDTH,
		int _SCREEN_HEIGHT);
private:
	const int titleHeight = 20; // percentage of screen from top
};

class GameScreen : public Screen {
public:
	GameScreen::GameScreen();

private:
	GameEntities** entities;
};


class GUIElement {
public:
	GUIElement(SDL_Renderer* _renderer, SDL_Rect _rect);
	virtual void render();
	void hide();
	void show();
	
protected:
	SDL_Renderer* renderer;
	SDL_Rect rect;
	bool hidden;
};


class Text : public GUIElement {
public:
	Text(SDL_Renderer* _renderer, SDL_Rect _rect, SDL_Surface* _textSurface, SDL_Surface* _charset, char* _text);
	void render();

private: 
	SDL_Surface* textSurface;
	SDL_Surface* charset;
	char* text;
};

class Button : public GUIElement {
public:
	Button(SDL_Renderer* _renderer, SDL_Rect _rect, Color _outlineColor, Color _fillColor, char* text);
	void render();

private:
	Color outlineColor;
	Color fillColor;
	char* text;
};