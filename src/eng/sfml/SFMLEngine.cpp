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
	sf::Event event;
    while (device.get()->pollEvent(event))
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
			device.get()->close();
    }

    // clear the window with black color
	device.get()->clear(sf::Color::Black);

    // draw everything here...
	for (auto node : nodeMap)
	{
		device.get()->draw(node.second);
	}

    // end the current frame
	device.get()->display();
}

void SFMLEngine::updateEntities(GameEngine& gameContext, std::vector<int> entitiesId) {

	for (int id : entitiesId) {
		SituationComponent& situation = gameContext.entityMan.getComponent<SituationComponent>(id);
		DrawableComponent& drawable = gameContext.entityMan.getComponent<DrawableComponent>(id);

		sf::Sprite& node = nodeMap[id];
		node.setPosition(situation.x, situation.y);
		node.setRotation(situation.rotation);
		node.setScale(situation.scaleX, situation.scaleY);
	}
}

void SFMLEngine::updateTextures(GameEngine& gameContext, std::vector<int> entitiesId) {
}

void SFMLEngine::createEntity(GameEngine& gameContext, int id) {
	DrawableComponent drawable    = gameContext.entityMan.getComponent<DrawableComponent>(id);
	SituationComponent situation  = gameContext.entityMan.getComponent<SituationComponent>(id);


	if (!existsTexture(drawable.sprite)) {
		addTexture(drawable.sprite);
	}

	nodeMap.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(textureMap[drawable.sprite]));

	sf::Sprite& node = nodeMap[id];
	node.setPosition(situation.x, situation.y);
	node.setRotation(situation.rotation);
	node.setScale(situation.scaleX, situation.scaleY);
}

void SFMLEngine::eraseEntity(int id) {
	nodeMap.erase(id);

	// Delete the texture and image?
}

void SFMLEngine::addColorToEntity(int id, std::array<float, 3> color) {
	nodeMap[id].setColor( sf::Color(color[0], color[1], color[2]) );
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
	if (!imageMap[path].loadFromFile(path)) {
		std::cout << "Error loading image " << path << "\n";
		imageMap.erase( prev(imageMap.end()) ); // IF error creating, delete the node
	}
}

bool SFMLEngine::existsTexture(std::string path) const {
	if (textureMap.find(path) == textureMap.end())
		return false;
	return true;
}

void SFMLEngine::addTexture(std::string path) {
	// Check if the image has been created
	if (!existsImage(path)) {
		addImage(path);
	}

	textureMap.emplace(std::piecewise_construct, std::forward_as_tuple(path), std::forward_as_tuple());
	if (!textureMap[path].loadFromImage( imageMap[path] )) {
		std::cout << "Error loading texture " << path << "\n";
		textureMap.erase( prev(textureMap.end()) ); // IF error creating, delete the node
	}
}