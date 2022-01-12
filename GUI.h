#pragma once
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "base_types.h"
#include "entity.h"
#include "game.h"
#include "settings.h"

#ifndef SCREEN_ENUM
#define SCREEN_ENUM
enum SCREEN { MAIN_MENU, LEVEL_1, LEVEL_2, LEVEL_3, QUIT };
#endif

class GUIElement; // forward declaration
class Game; // forward declaration
class Text;
class ScoreCounter;


class Screen {
public:
	Screen::Screen(
		SDL_Renderer* _renderer,
		SDL_Surface* _textSurface,
		SDL_Surface* _charset,
		SCREEN _screenId);
	virtual ~Screen();
	virtual void render();
	virtual void update(double delta, double worldTime) {};
	virtual void handleEvent(SDL_Event& event);
	void addElement(GUIElement* element);
	void hide();
	void show();

protected:
	SDL_Renderer* renderer;
	SDL_Surface* textSurface;
	SDL_Surface* charset;
	GUIElement** elements;
	SCREEN screenId;
	unsigned int elementsNumber = 10;
	unsigned int elementsCount = 0;
	bool hidden = false;
	bool exit = false;

	Color defaultButtonOutline = Color(191, 180, 178);
	Color defaultButtonFill = Color(40, 40, 40);
};


class MainMenu : public Screen {
public:
	MainMenu(
		SDL_Renderer* _renderer,
		SDL_Surface* _textSurface,
		SDL_Surface* _charset,
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
		Game* _game,
		SCREEN levelId,
		ScoreCounter* _score);
	~GameScreen();

	enum STATE { LOST, WON };
	void handleEvent(SDL_Event& event);
	void update(double delta, double worldTime);
	void render();
	void popup(STATE state);
private:
	SDL_Texture* background;
	int bgWidth, bgHeight;
	GameEntities entities;
	Player* player;
	Camera camera;
	Game* game;
	SCREEN currentLevel;
	ScoreCounter* score;
	Text* scoreText;

	char text[128];
	char* lostButtonPaths[2] = {
		"./assets/menu.bmp",
		"./assets/try_again.bmp"
	};
	char* wonButtonPaths[2] = {
		"./assets/next_level.bmp",
		"./assets/save_score.bmp"
	};
	char* lostTitle = "./assets/lost.bmp";
	char* wonTitle = "./assets/won.bmp";
	const int titleHeight = 20; // percentage of screen from top
	const int buttonWidth = 300;
	const int buttonHeight = 125;
	const int buttonMargin = 10;
	bool pause = false;

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
	char* text;

	Text(SDL_Renderer* _renderer, SDL_Rect _rect, SDL_Surface* _textSurface, SDL_Surface* _charset, char* _text);
	Text(SDL_Renderer* _renderer, SDL_Rect _rect, SDL_Surface* _textSurface, SDL_Surface* _charset, int maxChars);
	Text::~Text();
	
	void render();

private:
	SDL_Surface* textSurface;
	SDL_Surface* charset;
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


class ScoreCounter {
public:
	double MULTIPLIER_INCREASE = 0.1;
	int MULTIPLIER_TIME = 2500; // msec to start depleting

	void addScore(int points) {
		score += points * multiplier;
		updateText();
		increaseMultiplier();
	}

	void removeScore(int points) {
		score -= points;
		updateText();
	}

	void increaseMultiplier() {
		multiplier += MULTIPLIER_INCREASE;
		multiplierTimer.start();
		printf_s("%f\n", multiplier);
	}

	void decreaseMultiplier() {
		if (multiplier > 1.0) {
			multiplier -= MULTIPLIER_INCREASE;
			multiplier = multiplier < 1.0 ? 1.0 : multiplier;
		}
	}

	int getScore() {
		return score;
	}

	void update(double delta) {
		if (multiplierTimer.update(delta)) {
			decreaseMultiplier();
			if (multiplier > 1.0)
				multiplierTimer.start();
		}
	}

	void updateText() {
		sprintf(scoreText->text, "%d", score);
	}

	void setText(Text* _text) {
		scoreText = _text;
	}

private:
	Timer multiplierTimer = Timer(MULTIPLIER_TIME);
	Text* scoreText;
	int score = 0;
	double multiplier = 1.0;
};

