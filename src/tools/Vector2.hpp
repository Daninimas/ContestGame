#pragma once

struct Vector2 {
	Vector2(float posX, float posY)
		: x{ posX }, y{ posY } {};

	float x{ 0.f };
	float y{ 0.f };
};