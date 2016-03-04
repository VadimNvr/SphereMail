#pragma once

#include <cmath>
#include <cfloat>

struct Vector2D
{
	Vector2D(float pos_x = 0, float pos_y = 0) : x(pos_x), y(pos_y) {}

	float length() const { return sqrtf(powf(x, 2) + powf(y, 2)); }

	Vector2D operator + (const Vector2D &b) { return Vector2D(x + b.x, y + b.y); }
	Vector2D operator - (const Vector2D &b) { return Vector2D(x - b.x, y - b.y); }
	Vector2D operator * (float a) { return Vector2D(x * a, y * a); }
	Vector2D operator / (float a) { return Vector2D(x / a, y / a); }

	float x, y;
};