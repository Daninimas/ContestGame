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

	// Render collidables
	if (renderCollidables) {
		renderColliders(gameContext);
	}

    // end the current frame
	device.get()->display();
}

void SFMLEngine::renderColliders(GameEngine& gameContext) const {
	auto& colliders = gameContext.entityMan.getComponents<ColliderComponent>();

	for (ColliderComponent& c : colliders) {
		SituationComponent& sit = gameContext.entityMan.getComponent<SituationComponent>(c.id);
		BoundingBoxNode& b = c.boundingRoot;
		drawBoundingTree(b, sit);
	}
}

void SFMLEngine::drawBoundingTree(BoundingBoxNode boundingNode, SituationComponent& sit) const {
	// Draw this bounding box
	sf::RectangleShape rectangle(sf::Vector2f(boundingNode.bounding.xRight - boundingNode.bounding.xLeft, boundingNode.bounding.yDown - boundingNode.bounding.yUp));
	rectangle.setFillColor(sf::Color(0, 0, 0, 0));
	if (boundingNode.bounding.entitiesColliding.size() != 0) {
		rectangle.setFillColor(sf::Color(255, 0, 0));
	}
	rectangle.setOutlineThickness(2);
	rectangle.setOutlineColor(sf::Color(250, 150, 100));
	rectangle.setPosition(sit.x + boundingNode.bounding.xLeft, sit.y + boundingNode.bounding.yUp);
	device.get()->draw(rectangle);


	// Chall children to draw
	for (auto& b : boundingNode.childs) {
		drawBoundingTree(b, sit);
	}
}

void SFMLEngine::updateEntities(GameEngine& gameContext, std::vector<int> entitiesId) {

	for (int id : entitiesId) {
		if (existsNode(id)) {
			updateNode(gameContext, id);
		}
	}
}

void SFMLEngine::updateNode(GameEngine& gameContext, int id) {
	SituationComponent& situation = gameContext.entityMan.getComponent<SituationComponent>(id);
	RenderComponent& drawable = gameContext.entityMan.getComponent<RenderComponent>(id);
	sf::Sprite& node = nodeMap[id];

	node.setPosition(situation.x, situation.y);
	node.setRotation(situation.rotation);
	node.setScale(situation.scaleX, situation.scaleY);
	if (situation.facing == SituationComponent::Left) {
		node.move(node.getLocalBounds().width, 0.f);
		node.scale(-1.f, 1.f);
	}
}

void SFMLEngine::updateTextures(GameEngine& gameContext, std::vector<int> entitiesId) {
}

void SFMLEngine::createEntity(GameEngine& gameContext, int id) {
	RenderComponent& drawable    = gameContext.entityMan.getComponent<RenderComponent>(id);


	if (!existsTexture(drawable.sprite)) {
		addTexture(drawable.sprite);
	}

	nodeMap.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(textureMap[drawable.sprite]));

	// Set the position and data
	updateNode(gameContext, id);
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