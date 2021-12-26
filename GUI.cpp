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
	SDL_Rect titleRect;
	titleRect.x = SCREEN_WIDTH/2;
	titleRect.y = SCREEN_HEIGHT/2;
	titleRect.w = 100;
	titleRect.h = 100;
	Text* title = new Text(renderer, titleRect, textSurface, charset, "Wojna wydzialow");
	addElement(title);

}

GUIElement::GUIElement(SDL_Renderer* _renderer, SDL_Rect _rect) {
	renderer = _renderer;
	rect = _rect;
	hidden = false;
}

void GUIElement::render() { }

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


Button::Button(SDL_Renderer* _renderer, SDL_Rect _rect, Color _outlineColor, Color _fillColor, char* _text)
	:GUIElement(_renderer, _rect) {
	outlineColor = _outlineColor;
	fillColor = _fillColor;
	text = _text;
}


void Button::render() {
	if (!hidden) {
		drawRectangle(renderer, &rect, outlineColor, fillColor);
		//DrawString()
	}
}

