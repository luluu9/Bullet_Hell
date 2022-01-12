#include "base_types.h"
#include "math.h"

#define PI 3.14159265


Vector2::Vector2() {
	x = 0;
	y = 0;
}

Vector2::Vector2(float _x, float _y) {
	x = _x;
	y = _y;
}

Vector2 Vector2::normalized() {
	float magnitude = sqrtf(x * x + y * y);
	return Vector2(x / magnitude, y / magnitude);
}

float Vector2::getLength() {
	return sqrtf(x * x + y * y);
}

float Vector2::getAngleTo(Vector2 destVec) {
	return atan2(y-destVec.y,  x-destVec.x) * 180 / PI + 180;
}

float Vector2::getDistanceTo(Vector2 destVec) {
	return sqrtf(pow(x - destVec.x, 2) + pow(y - destVec.y, 2));
}


Color::Color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a)
	: r{ _r }, g{ _g }, b{ _b }, a{ _a } { 
	r = (r <= -255 ? -255 : r >= 255 ? 255 : r);
	g = (g <= -255 ? -255 : g >= 255 ? 255 : g);
	b = (b <= -255 ? -255 : b >= 255 ? 255 : b);
};

Color::Color(uint8_t _r, uint8_t _g, uint8_t _b)
	:Color(_r, _g, _b, 255) { };


Timer::Timer() {
	autoLoad = false;
	autoStart = false;
	resetOnStart = false;
	elapsedTime = 0.0;
	started = false;
}

// time in msec
Timer::Timer(double _time) {
	time = _time;
	autoLoad = false;
	autoStart = false;
	resetOnStart = false;
	elapsedTime = 0.0;
	started = false;
}

Timer::Timer(double _time, bool _autoLoad, bool _autoStart, bool _resetOnStart) {
	time = _time;
	autoLoad = _autoLoad;
	autoStart = _autoStart;
	resetOnStart = _resetOnStart;
	elapsedTime = 0.0;
	started = false;
}

void Timer::start() {
	if (!started) {
		elapsedTime = 0;
		started = true;
	}
	else if (resetOnStart) {
		elapsedTime = 0;
	}
}

void Timer::end() {
	started = false;
	if (autoStart)
		start();
}

void Timer::resume() {
	started = true;
}
void Timer::stop() {
	started = false;
}

// returns true on end
bool Timer::update(double delta) {
	if (started) {
		elapsedTime += delta*1000;
		if (elapsedTime >= time) {
			elapsedTime = time;
			end();
			return true;
		}
	}
	else if (autoLoad) {
		if (elapsedTime > 0)
			elapsedTime -= delta * 1000;
		else
			elapsedTime = 0;
	}
	return false;
}