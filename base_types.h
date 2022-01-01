#pragma once
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"

struct Vector2 {
	float x;
	float y;

	Vector2();
	Vector2(float _x, float _y);
	Vector2 normalized();
	float getLength();
	float getAngleTo(Vector2 destVec);
	float getDistanceTo(Vector2 destVec);
};

struct Color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	Color() : r{ 0 }, b{ 0 }, g{ 0 }, a{ 0 } {};
	Color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a);
	Color(uint8_t _r, uint8_t _g, uint8_t _b);
	
	operator bool() { return r || g || b; }
	Color operator *(const double m) { return Color(r * m, g * m, b * m); }
};


struct Timer {
	double elapsedTime;
	double time;
	bool started;
	bool autoLoad; // if stopped substract delta time
	bool autoStart; // if ended start again

	Timer(double _time, bool _autoLoad, bool _autoStart = false);
	Timer(double _time);
	void start();
	void end();
	void resume();
	void stop();
	bool update(double delta);

};