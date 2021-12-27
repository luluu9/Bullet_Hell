#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "GUI.h"
#include "drawing.h"
#include "stdio.h"


Screen::Screen(
	SDL_Renderer* _renderer,
	SDL_Surface* _textSurface,
	SDL_Surface* _charset,
	int _SCREEN_WIDTH,
	int _SCREEN_HEIGHT) {
	SCREEN_WIDTH = _SCREEN_WIDTH;
	SCREEN_HEIGHT = _SCREEN_HEIGHT;
	renderer = _renderer;
	textSurface = _textSurface;
	charset = _charset;
	elements = new GUIElement * [elementsNumber];
}

void Screen::render() {
	for (unsigned int i = 0; i < elementsCount; i++) {
		elements[i]->render();
	}
}

void Screen::handleEvent(SDL_Event& event) {
	for (unsigned int i = 0; i < elementsCount; i++) {
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


MainMenu::MainMenu(
	SDL_Renderer* _renderer,
	SDL_Surface* _textSurface,
	SDL_Surface* _charset,
	int _SCREEN_WIDTH,
	int _SCREEN_HEIGHT)
	:Screen::Screen(
		_renderer,
		_textSurface,
		_charset,
		_SCREEN_WIDTH,
		_SCREEN_HEIGHT) {
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
		Button* button = new Button(renderer, buttonRect, Color(191, 180, 178), Color(40, 40, 40), buttonPaths[i]);
		addElement(button);
		buttonRect.y += buttonHeight * 1.25;
	}
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
	char* texturePath)
	:GUIElement(_renderer, _rect) {
	outlineColor = _outlineColor;
	fillColor = baseFillColor = _fillColor;
	hoverColor = _fillColor*2.0;
	image = new Image(renderer, rect, texturePath);
}


void Button::render() {
	if (!hidden) {
		drawRectangle(renderer, rect, outlineColor, fillColor, true);
		image->render();
	}
}

void Button::handleEvent(SDL_Event& event) {
	if (event.type == SDL_MOUSEMOTION) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		if (x >= rect.x - rect.w / 2 &&
			x <= rect.x + rect.w / 2 &&
			y >= rect.y - rect.h / 2 &&
			y <= rect.y + rect.h / 2) {
			fillColor = hoverColor;
		}
		else {
			fillColor = baseFillColor;
		}
	}
}

