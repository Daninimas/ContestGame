#pragma once
#include <string>
#include <tools/Color.hpp>
#include <tools/Vector2.hpp>

struct BackgroundLayer
{
	BackgroundLayer() = default;
	BackgroundLayer(std::string sprite)
		: layerSprite{ sprite }, layerColor{ 255, 255, 255, 255 } {};
	BackgroundLayer(std::string sprite, Color color)
		: layerSprite{ sprite }, layerColor{color} {};
	BackgroundLayer(std::string sprite, Vector2 position, Color color)
		: layerSprite{ sprite }, layerPosition{ position }, layerColor{ color } {};

	std::string layerSprite;
	Vector2		layerPosition;
	Color	    layerColor;
};