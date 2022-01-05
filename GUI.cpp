#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "GUI.h"
#include "drawing.h"
#include "settings.h"
#include "stdio.h"
#include <iostream>


Screen::Screen(
	SDL_Renderer* _renderer,
	SDL_Surface* _textSurface,
	SDL_Surface* _charset,
	SCREEN _screenId) {
	renderer = _renderer;
	textSurface = _textSurface;
	charset = _charset;
	elements = new GUIElement * [elementsNumber];
	screenId = _screenId;
}

Screen::~Screen() {
	printf("Deleting screen\n");
	for (unsigned int i = 0; i < elementsCount; i++) {
		elements[i]->~GUIElement();
	}
	for (unsigned int i = 0; i < elementsCount; i++) {
		delete elements[i];
	}
	delete[] elements;
	exit = true; // exit each loop
}

void Screen::render() {
	if (hidden) return;
	for (unsigned int i = 0; i < elementsCount; i++) {
		if (exit) return;
		elements[i]->render();
	}
}

void Screen::handleEvent(SDL_Event& event) {
	if (hidden) return;
	for (unsigned int i = 0; i < elementsCount; i++) {
		if (exit) return;
		elements[i]->handleEvent(event);
	}
}

void Screen::addElement(GUIElement* element) {
	if (elementsCount >= elementsNumber) {
		elementsNumber *= 2;
		GUIElement** extendedArray = new GUIElement * [elementsNumber];
		for (unsigned int i = 0; i < elementsCount; i++)
			extendedArray[i] = elements[i];
		delete[] elements;
		elements = extendedArray;
	}
	elements[elementsCount] = element;
	elementsCount++;
}

void Screen::hide() {
	hidden = true;
}

void Screen::show() {
	hidden = false;
}


MainMenu::MainMenu(
	SDL_Renderer* _renderer,
	SDL_Surface* _textSurface,
	SDL_Surface* _charset,
	Game* _game)
	:Screen::Screen(
		_renderer,
		_textSurface,
		_charset,
		MAIN_MENU) {
	game = _game;
	SDL_Rect titleRect, buttonRect;
	titleRect.x = SCREEN_WIDTH / 2;
	titleRect.y = SCREEN_HEIGHT / 100 * titleHeight;
	Image* title = new Image(renderer, titleRect, titlePath);
	addElement(title);

	buttonRect.w = buttonWidth;
	buttonRect.h = buttonHeight;
	buttonRect.x = SCREEN_WIDTH / 2;
	buttonRect.y = SCREEN_HEIGHT / 100 * firstButtonHeight;
	for (int i = 0; i < buttonsNumber; i++) {
		Button* button = new Button(renderer, buttonRect, defaultButtonOutline, defaultButtonFill, buttonPaths[i], SCREEN(LEVEL_1 + i), game);
		addElement(button);
		buttonRect.y += buttonHeight * 1.25;
	}
}


GameScreen::GameScreen(
	SDL_Renderer* _renderer,
	SDL_Surface* _textSurface,
	SDL_Surface* _charset,
	Game* _game,
	SCREEN levelId)
	:Screen::Screen(
		_renderer,
		_textSurface,
		_charset,
		levelId) {
	game = _game;
	currentLevel = levelId;

	camera.x = 0;
	camera.y = 0;
	camera.w = SCREEN_WIDTH;
	camera.h = SCREEN_HEIGHT;

	charset = SDL_LoadBMP("./cs8x8.bmp");
	SDL_SetColorKey(charset, true, 0xFF000000);

	player = new Player(renderer, &camera, game->getKeyboard(), &entities);
	entities.addEntity(player);

	background = loadTextureFromBMP(renderer, BACKGROUND_TXT_PATH);
	SDL_QueryTexture(background, NULL, NULL, &bgWidth, &bgHeight);

	switch (currentLevel) {
		case LEVEL_1: {
			for (int i = 0; i < 1; i++) {
				Chemiczny* enemy = new Chemiczny(renderer, &camera, &entities, player);
				entities.addEntity(enemy);
				enemy->setPos(Vector2(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT));
			}
			break;
		}
		case LEVEL_2: {
			AIR* air = new AIR(renderer, &camera, &entities, player);
			entities.addEntity(air);
			air->setPos(Vector2(400, 400));
			break;
		}
		case LEVEL_3: {
			WILIS* wilis = new WILIS(renderer, &camera, &entities, player);
			entities.addEntity(wilis);
			wilis->setPos(Vector2(500, 500));
			break;
		}
	}
}

GameScreen::~GameScreen() {
	printf("Deleting gamescreen\n");
}

void GameScreen::handleEvent(SDL_Event& event) {
	Screen::handleEvent(event);
	if (pause) return;
	for (unsigned int i = 0; i < entities.currentEntity; i++) {
		entities.getEntity(i)->handleEvent(event);
	}
}

void GameScreen::update(double delta, double worldTime) {
	if (pause) return;
	entities.removeQueuedEntities();
	int enemies = 0;
	SDL_Event x;
	for (unsigned int i = 0; i < entities.currentEntity; i++) {
		Entity* currentEntity = entities.getEntity(i);
		currentEntity->handleEvent(x);
		currentEntity->update(delta);
		if (currentEntity->entityType == ENEMY)
			enemies++;
	}

	player->colliding = false;
	for (unsigned int i = 0; i < entities.currentEntity; i++) {
		Entity* currentEntity = entities.getEntity(i);
		if (currentEntity != player) {
			if (isColliding(player, currentEntity)) {
				player->collide(currentEntity, delta);
			}
		}
	}

	sprintf_s(text, "% .1lf s ", worldTime);
	DrawString(textSurface, SCREEN_WIDTH / 2, 10, text, charset);

	if (enemies == 0) {
		popup(WON);
	}
	if (player->getHP() <= 0.0) {
		popup(LOST);
	}
}

void GameScreen::render() {
	for (int i = -1; i <= SCREEN_WIDTH / bgWidth + 2; i++)
		for (int j = -1; j <= SCREEN_HEIGHT / bgHeight + 2; j++)
			DrawTexture(renderer, background, -((int)camera.x % bgWidth) + i * bgWidth, -((int)camera.y % bgHeight) + j * bgHeight);
	for (unsigned int i = 0; i < entities.currentEntity; i++)
		entities.getEntity(i)->render();

	if (pause) {
		SDL_Rect shadowRect;
		shadowRect.x = 0;
		shadowRect.y = 0;
		shadowRect.w = SCREEN_WIDTH;
		shadowRect.h = SCREEN_HEIGHT;
		drawRectangle(renderer, shadowRect, Color(0, 0, 0), Color(-255, -255, -255, 100));
	}
	Screen::render(); // draw last to always be visible
}

void GameScreen::popup(STATE state) {
	SDL_Rect titleRect, buttonRect;
	Image* title;
	Button* button;
	titleRect.x = SCREEN_WIDTH / 2;
	titleRect.y = SCREEN_HEIGHT / 100 * titleHeight;
	if (state == LOST)
		title = new Image(renderer, titleRect, lostTitle);
	else
		title = new Image(renderer, titleRect, wonTitle);
	addElement(title);

	buttonRect.w = buttonWidth;
	buttonRect.h = buttonHeight;
	buttonRect.x = SCREEN_WIDTH / 2 - buttonWidth / 2 - buttonMargin;
	buttonRect.y = SCREEN_HEIGHT / 2;
	SCREEN nextLevel = (SCREEN)((int)currentLevel + 1);
	if (state == LOST)
		button = new Button(renderer, buttonRect, defaultButtonOutline, defaultButtonFill, lostButtonPaths[0], MAIN_MENU, game);
	else
		button = new Button(renderer, buttonRect, defaultButtonOutline, defaultButtonFill, wonButtonPaths[0], nextLevel, game);
	addElement(button);

	buttonRect.x += buttonWidth + buttonMargin;
	if (state == LOST)
		button = new Button(renderer, buttonRect, defaultButtonOutline, defaultButtonFill, lostButtonPaths[1], currentLevel, game);
	else
		button = new Button(renderer, buttonRect, defaultButtonOutline, defaultButtonFill, wonButtonPaths[1], MAIN_MENU, game); // change to SCOREBOARD if implemented
	addElement(button);
	pause = true;
}


GUIElement::GUIElement(SDL_Renderer* _renderer, SDL_Rect _rect) {
	renderer = _renderer;
	rect = _rect;
	hidden = false;
}

void GUIElement::hide() {
	hidden = true;
}

void GUIElement::show() {
	hidden = false;
}


Text::Text(
	SDL_Renderer* _renderer,
	SDL_Rect _rect,
	SDL_Surface* _textSurface,
	SDL_Surface* _charset,
	char* _text)
	:GUIElement::GUIElement(_renderer, _rect) {
	textSurface = _textSurface;
	charset = _charset;
	text = _text;
}

void Text::render() {
	if (!hidden) {
		DrawString(textSurface, rect.x, rect.y, text, charset);
	}
}


Image::Image(
	SDL_Renderer* _renderer,
	SDL_Rect _rect,
	char* texturePath)
	:GUIElement(_renderer, _rect) {
	image = loadTextureFromBMP(renderer, texturePath);
}

Image::~Image() {
	SDL_DestroyTexture(image);
}

void Image::render() {
	if (!hidden) {
		DrawTexture(renderer, image, rect.x, rect.y);
	}
}


Button::Button(
	SDL_Renderer* _renderer,
	SDL_Rect _rect,
	Color _outlineColor,
	Color _fillColor,
	char* texturePath,
	SCREEN _nextScreenId,
	Game* _game)
	:GUIElement(_renderer, _rect) {
	outlineColor = _outlineColor;
	fillColor = baseFillColor = _fillColor;
	hoverColor = _fillColor * 2.0;
	image = new Image(renderer, rect, texturePath);
	nextScreenId = _nextScreenId;
	game = _game;
}

Button::~Button() {
	delete image;
}

void Button::render() {
	if (!hidden) {
		drawRectangle(renderer, rect, outlineColor, fillColor, true);
		image->render();
	}
}

void Button::handleEvent(SDL_Event& event) {
	if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONUP) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		if (x >= rect.x - rect.w / 2 &&
			x <= rect.x + rect.w / 2 &&
			y >= rect.y - rect.h / 2 &&
			y <= rect.y + rect.h / 2) {
			fillColor = hoverColor;
			if (event.type == SDL_MOUSEBUTTONUP) {
				game->changeScreen(nextScreenId);
			}
		}
		else {
			fillColor = baseFillColor;
		}
	}
}

