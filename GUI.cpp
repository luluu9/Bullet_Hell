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
	SCREEN levelId,
	ScoreCounter* _score)
	:Screen(
		_renderer,
		_textSurface,
		_charset,
		levelId) {
	game = _game;
	currentLevel = levelId;
	score = _score;

	loadSettings(currentLevel);

	camera.x = 0;
	camera.y = 0;
	camera.w = SCREEN_WIDTH;
	camera.h = SCREEN_HEIGHT;

	SDL_Rect textRect;
	textRect.x = SCREEN_WIDTH / 2;
	textRect.y = scoreY;
	scoreText = new Text(renderer, textRect, textSurface, charset, MAX_CHARS);
	addElement(scoreText);
	score->setText(scoreText);
	score->updateText();

	player = new Player(renderer, &camera, game->getKeyboard(), &entities, score);
	entities.addEntity(player);

	background = loadTextureFromBMP(renderer, BACKGROUND_TXT_PATH);
	SDL_QueryTexture(background, NULL, NULL, &bgWidth, &bgHeight);

	if (LEVEL_CHEMICZNY) {
		Chemiczny* enemy = new Chemiczny(renderer, &camera, &entities, player);
		entities.addEntity(enemy);
	}
	if (LEVEL_AIR) {
		AIR* air = new AIR(renderer, &camera, &entities, player);
		entities.addEntity(air);
	}
	if (LEVEL_WILIS) {
		WILIS* wilis = new WILIS(renderer, &camera, &entities, player);
		entities.addEntity(wilis);
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

	score->update(delta);

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
			drawTexture(renderer, background, -((int)camera.x % bgWidth) + i * bgWidth, -((int)camera.y % bgHeight) + j * bgHeight);
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
	score->drawGrade();
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
		button = new Button(renderer, buttonRect, defaultButtonOutline, defaultButtonFill, wonButtonPaths[1], SCOREBOARD, game);
	addElement(button);
	pause = true;
}


Scoreboard::Scoreboard(
	SDL_Renderer* _renderer,
	SDL_Surface* _textSurface,
	SDL_Surface* _charset,
	Game* _game,
	SCREEN levelId,
	ScoreCounter* _score)
	:Screen(
		_renderer,
		_textSurface,
		_charset,
		levelId) {
	game = _game;
	currentLevel = levelId;
	score = _score;

	SDL_Rect titleRect;
	titleRect.x = SCREEN_WIDTH / 2;
	titleRect.y = SCREEN_HEIGHT / 100 * titleHeight;
	Image* title = new Image(renderer, titleRect, SCOREBOARD_PATH);
	addElement(title);

	SDL_Rect textRect;
	textRect.x = SCREEN_WIDTH / 2;
	textRect.y = SCREEN_HEIGHT / 3;
	textRect.w = inputWidth;
	textRect.h = inputHeight;
	textInput = new TextInput(renderer, textRect, textSurface, charset, defaultButtonOutline, defaultButtonFill);
	addElement(textInput);
}


Scoreboard::~Scoreboard() {

}

void Scoreboard::handleEvent(SDL_Event& event) {
	switch (event.type) {
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_BACKSPACE){
			saveScore();
			showScores();
			return;
		}
		break;
	};
	Screen::handleEvent(event);
}

void Scoreboard::render() {
	if (!hidden)
		Screen::render();
}

void Scoreboard::saveScore() {
	printf_s("%s\n", textInput->text);
	textInput->hide();
}

void Scoreboard::showScores() {

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

Text::Text(
	SDL_Renderer* _renderer,
	SDL_Rect _rect,
	SDL_Surface* _textSurface,
	SDL_Surface* _charset,
	int maxChars)
	:GUIElement::GUIElement(_renderer, _rect) {
	textSurface = _textSurface;
	charset = _charset;
	text = new char[maxChars + 1];
	for (int i = 0; i < maxChars; i++)
		text[i] = ' ';
	text[maxChars] = '\0';
}

Text::~Text() {
	delete[] text;
}

void Text::render() {
	if (!hidden) {
		drawString(textSurface, rect.x - strlen(text) * DEST_CHAR_SIZE / 2, rect.y, text, charset);
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
		drawTexture(renderer, image, rect.x, rect.y);
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


ScoreCounter::ScoreCounter(SDL_Renderer* _renderer) {
	renderer = _renderer;
}

void ScoreCounter::addScore(int points) {
	score += points * multiplier;
	updateText();
	increaseMultiplier();
}

void ScoreCounter::removeScore(int points) {
	if (score > 0)
		score = score - points > 0 ? score - points : 0;
	updateText();
}

void ScoreCounter::increaseMultiplier() {
	if (multiplier < MAX_MULTIPLIER)
		multiplier += MULTIPLIER_INCREASE;
	multiplierTimer.start();
}

void ScoreCounter::resetMultiplier() {
	multiplier = 1.0;
}

void ScoreCounter::update(double delta) {
	if (multiplierTimer.update(delta)) {
		//resetMultiplier();
		resetTimer.start();
	}
	if (resetTimer.update(delta)) {
		multiplier -= 1.0;
		if (multiplier <= 1.0) {
			resetTimer.stop();
			multiplier = 1.0;
		}
	}
}

void ScoreCounter::updateText() {
	sprintf(scoreText->text, "%d", score);
}

void ScoreCounter::setText(Text* _text) {
	scoreText = _text;
}

void ScoreCounter::drawGrade() {
	SDL_Rect windowRect;
	windowRect.x = 0;
	windowRect.y = 0;
	windowRect.w = SCREEN_WIDTH;
	windowRect.h = SCREEN_HEIGHT;
	drawRectangle(renderer, windowRect, Color(), Color(255, 0, 0, multiplier * 1.5));
}

int ScoreCounter::getScore() {
	return score;
}


TextInput::TextInput(
	SDL_Renderer* _renderer,
	SDL_Rect _rect,
	SDL_Surface* _textSurface,
	SDL_Surface* _charset,
	Color _outlineColor,
	Color _fillColor)
	:Text(_renderer, _rect, _textSurface, _charset, MAX_CHARS) {
	outlineColor = _outlineColor;
	fillColor = _fillColor;
	bgRect = _rect;
	bgRect.y += rect.h / 4;
}

void TextInput::render() {
	if (hidden) return;
	Text::render();
	drawRectangle(renderer, bgRect, outlineColor, fillColor, true);
}

void TextInput::handleEvent(SDL_Event& event) {
	if (hidden) return;
	switch (event.type) {
	case SDL_KEYDOWN:
		SDL_Keycode currKey = event.key.keysym.sym;
		char newChar = 'A' + (currKey - SDLK_a);
		if ('A' <= newChar && newChar <= 'Z')
			addChar(newChar);
		else if (currKey == SDLK_SPACE)
			addChar(' ');
		else if (currKey == SDLK_BACKSPACE)
			removeLastChar();
		printf_s("%s\n", text);
		break;
	};
}

void TextInput::removeLastChar() {
	if (currChar > 0) {
		text[currChar - 1] = '\0';
		currChar--;
	}
}

void TextInput::addChar(char c) {
	if (currChar < MAX_CHARS) {
		text[currChar] = c;
		text[currChar + 1] = '\0';
		currChar++;
	}
}