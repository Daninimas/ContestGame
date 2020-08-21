#include "SFMLEngine.hpp"

#include <eng/GameEngine.hpp>
#include <iostream>

SFMLEngine::SFMLEngine(int width, int height, bool fullscreen) {

	if (fullscreen) {
		device = std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), "Game", sf::Style::Fullscreen);
	}
	else {
		device = std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), "Game");
	}
}
SFMLEngine::~SFMLEngine() {

}

const bool SFMLEngine::checkInput(const uint16_t key) const {
	return false;
}

std::array<float, 2> SFMLEngine::getMousePosition() const {
	return std::array<float, 2>();
}

std::array<float, 2> SFMLEngine::getWindowDimensions() const {
	return std::array<float, 2>();
}

void SFMLEngine::render(GameEngine& gameContext) const {
}

void SFMLEngine::updateEntities(GameEngine& gameContext, std::vector<int> entitiesId) {
}

void SFMLEngine::updateTextures(GameEngine& gameContext, std::vector<int> entitiesId) {
}

void SFMLEngine::createEntity(GameEngine& gameContext, int id) {
	DrawableComponent drawable    = gameContext.entityMan.getComponent<DrawableComponent>(id);
	SituationComponent situation = gameContext.entityMan.getComponent<SituationComponent>(id);

	if (!existsImage(drawable.sprite)) {
		addImage(drawable.sprite);
	}

	sf::Texture texture;
	texture.loadFromImage(imageMap[drawable.sprite]);

	nodeMap.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(texture));
}

void SFMLEngine::eraseEntity(int id) {
}

void SFMLEngine::addColorToEntity(int id, std::array<float, 3> color) {
}

size_t SFMLEngine::countRenderNodes() const {
	return nodeMap.size();
}

bool SFMLEngine::existsNode(int id) const {
	if (nodeMap.find(id) == nodeMap.end())
		return false;
	return true;
}

bool SFMLEngine::existsImage(std::string path) const {
	if (imageMap.find(path) == imageMap.end())
		return false;
	return true;
}

void SFMLEngine::addImage(std::string path) {
	imageMap.emplace(std::piecewise_construct, std::forward_as_tuple(path), std::forward_as_tuple(  ) );
	if (!imageMap.end()->second.loadFromFile("background.jpg")) {
		std::cout << "Error loading image " << path << "\n";
		imageMap.erase(imageMap.end()); // IF error creating, delete the node
	}
}
/*
bool SFMLEngine::existsTexture(std::string path) const {
	if (textureMap.find(path) == textureMap.end())
		return false;
	return true;
}

void SFMLEngine::addTexture(std::string path) {
	textureMap.emplace(std::piecewise_construct, std::forward_as_tuple(path), std::forward_as_tuple());
	if (!textureMap.end()->second.loadFromFile("background.jpg")) {
		std::cout << "Error loading image " << path << "\n";
		imageMap.erase(imageMap.end()); // IF error creating, delete the node
	}
}*/