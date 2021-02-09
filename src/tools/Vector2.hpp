#pragma once

struct Vector2 {
	Vector2() = default;
	Vector2(float posX, float posY)
		: x{ posX }, y{ posY } {};
	/*
    Vector2& operator+(const Vector2& other)
    {
		this->x += other.x;
		this->y += other.y;
        return *this;
    }

	Vector2& operator-(const Vector2& other)
	{
		this->x -= other.x;
		this->y -= other.y;
		return *this;
	}

	Vector2& operator*(const Vector2& other)
	{
		this->x *= other.x;
		this->y *= other.y;
		return *this;
	}

	Vector2& operator/(const Vector2& other)
	{
		this->x /= other.x;
		this->y /= other.y;
		return *this;
	}
	*/
	float x{ 0.f };
	float y{ 0.f };
};