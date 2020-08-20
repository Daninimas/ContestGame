#include "SFMLEngine.hpp"

SFMLEngine::SFMLEngine(int width, int height, bool fullscreen) {

	if (fullscreen) {
		device = std::make_unique<sf::Window>(sf::VideoMode(width, height), "Game", sf::Style::Fullscreen);
	}
	else {
		device = std::make_unique<sf::Window>(sf::VideoMode(width, height), "Game");
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
}

void SFMLEngine::eraseEntity(int id) {
}

void SFMLEngine::addColorToEntity(int id, std::array<float, 3> color) {
}

size_t SFMLEngine::countRenderNodes() const {
	return 0;
}

bool SFMLEngine::existsNode(int id) const {
	return 0;
}