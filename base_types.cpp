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
	r = (r <= 0 ? 0 : r >= 255 ? 255 : r);
	g = (g <= 0 ? 0 : g >= 255 ? 255 : g);
	b = (b <= 0 ? 0 : b >= 255 ? 255 : b);
};

Color::Color(uint8_t _r, uint8_t _g, uint8_t _b)
	:Color(_r, _g, _b, 0) { };


Timer::Timer(double _time, bool _autoReset) {
	time = _time;
	autoReset = _autoReset;
	elapsedTime = 0.0;
	started = false;
}

void Timer::start() {
	if (!started) {
		elapsedTime = 0;
		started = true;
	}
}

void Timer::end() {
	started = false;
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
	else if (autoReset) {
		if (elapsedTime > 0)
			elapsedTime -= delta * 1000;
		else
			elapsedTime = 0;
	}
	return false;
}