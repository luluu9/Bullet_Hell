#pragma once
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "base_types.h"
#include "entity.h"
#include "game.h"

#ifndef SCREEN_ENUM
#define SCREEN_ENUM
enum SCREEN { MAIN_MENU, LEVEL_1, LEVEL_2, LEVEL_3, QUIT };
#endif

class GUIElement; // forward declaration
class Game; // forward declaration


class Screen {
public:
	Screen::Screen(
		SDL_Renderer* _renderer,
		SDL_Surface* _textSurface,
		SDL_Surface* _charset,
		int _SCREEN_WIDTH,
		int _SCREEN_HEIGHT,
		SCREEN _screenId);
	virtual ~Screen();
	virtual void render();
	virtual void update(double delta, double worldTime) {};
	virtual void handleEvent(SDL_Event& event);
	void addElement(GUIElement* element);
	void hide();
	void show();

protected:
	int SCREEN_WIDTH, SCREEN_HEIGHT;
	SDL_Renderer* renderer;
	SDL_Surface* textSurface;
	SDL_Surface* charset;
	GUIElement** elements;
	SCREEN screenId;
	unsigned int elementsNumber = 10;
	unsigned int elementsCount = 0;
	bool hidden = false;
	bool exit = false;
};


class MainMenu : public Screen {
public:
	MainMenu(
		SDL_Renderer* _renderer,
		SDL_Surface* _textSurface,
		SDL_Surface* _charset,
		int _SCREEN_WIDTH,
		int _SCREEN_HEIGHT,
		Game* _game);
private:
	char* titlePath = "./assets/title.bmp";
	const int buttonsNumber = 4;
	char* buttonPaths[4] = {
		"./assets/level-1.bmp", "./assets/level-2.bmp",
		"./assets/level-3.bmp", "./assets/quit.bmp" };
	const int titleHeight = 20; // percentage of screen from top
	const int firstButtonHeight = 50;
	const int buttonWidth = 400;
	const int buttonHeight = 100;
	Game* game;
};


class GameScreen : public Screen {
public:
	GameScreen(
		SDL_Renderer* _renderer,
		SDL_Surface* _textSurface,
		SDL_Surface* _charset,
		int _SCREEN_WIDTH,
		int _SCREEN_HEIGHT,
		Game* _game,
		SCREEN levelId);
	~GameScreen();
	void handleEvent(SDL_Event& event);
	void update(double delta, double worldTime);
	void render();
private:
	SDL_Texture* background;
	int bgWidth, bgHeight;
	GameEntities entities;
	Player* player;
	SDL_Rect camera;
	Game* game;
	char text[128];

	AnimationPlayer* testPlayer;
};


class GUIElement {
public:
	GUIElement(SDL_Renderer* _renderer, SDL_Rect _rect);
	virtual ~GUIElement() {};
	virtual void render() {};
	virtual void handleEvent(SDL_Event& event) {};
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

class Image : public GUIElement {
public:
	Image(
		SDL_Renderer* _renderer,
		SDL_Rect _rect,
		char* texturePath);
	Image::~Image();
	void render();

private:
	SDL_Texture* image;
};

class Button : public GUIElement {
public:
	Button(
		SDL_Renderer* _renderer,
		SDL_Rect _rect,
		Color _outlineColor,
		Color _fillColor,
		char* texturePath,
		SCREEN _nextScreenId,
		Game* _game);
	Button::~Button();
	void render();
	void handleEvent(SDL_Event& event);

private:
	Color outlineColor;
	Color fillColor;
	Color baseFillColor;
	Color hoverColor;
	Image* image;
	SCREEN nextScreenId;
	Game* game;
};