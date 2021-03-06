#pragma once
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include <stdio.h>
#include <cstdlib>


struct Vector2 {
	float x;
	float y;

	Vector2();
	Vector2(float _x, float _y);
	Vector2 normalized();
	float getLength();
	float getAngleTo(Vector2 destVec);
	float getDistanceTo(Vector2 destVec);

	Vector2 operator *(double m) { return Vector2(x * m, y * m); }
	Vector2 operator /(double m) { return Vector2(x / m, y / m); }
	Vector2 operator -(Vector2 m) { return Vector2(x - m.x, y - m.y); }
	Vector2 operator +(Vector2 m) { return Vector2(x + m.x, y + m.y); }
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
	bool resetOnStart; // if start called reset timer

	Timer(double _time, bool _autoLoad, bool _autoStart = false, bool _resetOnStart = false);
	Timer(double _time);
	Timer();
	void start();
	void end();
	void resume();
	void stop();
	bool update(double delta);

};


struct Camera {
	float x;
	float y;
	float w;
	float h;
	Timer shakeTimer;
	int shakeMsec;
	int shakePower;
	bool shake = false;

	void startShake(int _shakeMsec, int _shakePower);
	void update(double delta);
	void setPos(Vector2 pos);

};