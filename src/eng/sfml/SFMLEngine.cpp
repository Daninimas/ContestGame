#include "SFMLEngine.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>
#include <iostream>

SFMLEngine::SFMLEngine(int width, int height, bool fullscreen) {

	if (fullscreen) {
		device = std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), "Game", sf::Style::Fullscreen);
	}
	else {
		device = std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), "Game", sf::Style::Titlebar | sf::Style::Close);
	}

	std::string fontPath = "Media/Fonts/Alien Android Italic.ttf";
	if (!font.loadFromFile(fontPath))
	{
		std::cout << "Could not load font: " << fontPath << "\n";
	}
}
SFMLEngine::~SFMLEngine() {
	device.get()->close();
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
		if (event.type == sf::Event::Closed) {
			device.get()->close();

			gameContext.setPlaying(false);
		}

		else if (event.type == sf::Event::LostFocus) {
			gameContext.pushGameState(GameState::PAUSE);
		}

		// For the change of controls
		if (gameContext.getGameState() == GameState::WAIT_INPUT && event.type == sf::Event::KeyPressed) {
			setKeyToControl(gameContext, event);
			gameContext.popGameState();
		}
    }

    // clear the window with black color
	device.get()->clear(sf::Color::Black);

	for (auto& cameraPair : cameraMap) {
		// Change the view to the camera view
		device.get()->setView(cameraPair.second);

		// Render scene from the view perspective
		drawScene(gameContext);
	}

	if (cameraMap.size() == 0) {
		// Render the scene without cameras
		drawScene(gameContext);
	}

	drawHudElements(gameContext);

    // end the current frame
	device.get()->display();
}

void SFMLEngine::drawScene(GameEngine& gameContext) const {
	// draw everything here...
	for (auto& node : nodeMap)
	{
		device.get()->draw(node.second);
	}

	// Render collidables
	if (renderCollidables) {
		renderColliders(gameContext);
	}
	// Render sensors
	if (renderSensors) {
		renderAllSensors(gameContext);
	}
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
		rectangle.setFillColor(sf::Color(255, 0, 0, 100));
	}
	rectangle.setOutlineThickness(2);
	rectangle.setOutlineColor(sf::Color(250, 150, 100));
	rectangle.setPosition(sit.position.x + boundingNode.bounding.xLeft, sit.position.y + boundingNode.bounding.yUp);
	device.get()->draw(rectangle);


	// Chall children to draw
	for (auto& b : boundingNode.childs) {
		drawBoundingTree(b, sit);
	}
}

void SFMLEngine::renderAllSensors(GameEngine& gameContext) const {
	auto& sensors = gameContext.entityMan.getComponents<SensorComponent>();

	for (SensorComponent& s : sensors) {
		SituationComponent& sit = gameContext.entityMan.getComponent<SituationComponent>(s.id);
		BoundingBox& b = s.sensorBounding;

		// Draw the sensor rectangle
		sf::RectangleShape rectangle(sf::Vector2f(b.xRight - b.xLeft, b.yDown - b.yUp));

		rectangle.setFillColor(sf::Color(0, 0, 0, 0));
		rectangle.setOutlineThickness(1);
		rectangle.setOutlineColor(sf::Color(0, 255, 0));

		if (sit.facing == SituationComponent::Right) {
			rectangle.setPosition(sit.position.x + b.xLeft, sit.position.y + b.yUp);
		}
		else {
			rectangle.setPosition(sit.position.x + b.xLeft - (b.xRight - b.xLeft), sit.position.y + b.yUp);

		}
		device.get()->draw(rectangle);
	}
}

void SFMLEngine::drawHudElements(GameEngine& gameContext) const {

	device.get()->setView(device.get()->getDefaultView()); //Change view to the hud view

	// Render all HUD sprites
	for (auto& node : HUDNodeMap)
	{
		device.get()->draw(node.second);
	}

	// Render all HUD text
	for (auto& text : HUDTextMap) 
	{
		device.get()->draw(text.second);
	}
}



void SFMLEngine::updateEntities(GameEngine& gameContext, std::vector<int> entitiesId) {

	for (int id : entitiesId) {
		if (existsNode(id)) {
			updateNode(gameContext, nodeMap[id], id);
		}
		else if (existsHUDNode(id)) {
			updateNode(gameContext, HUDNodeMap[id], id);
		}
		else if (existsHUDText(id)) {
			updateText(gameContext, HUDTextMap[id], id);
		}
		else if (existsCamera(id)) {
			updateCamera(gameContext, id);
		}
	}
}

void SFMLEngine::updateNode(GameEngine& gameContext, sf::Sprite& node, int id) {
	SituationComponent& situation = gameContext.entityMan.getComponent<SituationComponent>(id);
	RenderComponent& drawable = gameContext.entityMan.getComponent<RenderComponent>(id);

	node.setOrigin(0,0);

	node.setPosition(situation.position.x, situation.position.y);
	node.setRotation(situation.rotation);
	node.setScale(situation.scale.x, situation.scale.y);

	if (situation.facing == SituationComponent::Left) {
		node.move(node.getGlobalBounds().width, 0.f);
		node.scale(-1.f, 1.f);
	}
}

void SFMLEngine::setColorToEntity(const int id, const Color color) {
	if(existsNode(id))
		nodeMap[id].setColor(sf::Color(color.r, color.g, color.b, color.a));

	if(existsHUDNode(id))
		HUDNodeMap[id].setColor(sf::Color(color.r, color.g, color.b, color.a));
}


void SFMLEngine::updateTextures(GameEngine& gameContext, std::vector<int> entitiesId) {
	for (int id : entitiesId) {
		if (existsNode(id)) {
			updateTexture(gameContext, nodeMap[id], id);
		}
		else {
			// if dont exists, create node
			createEntity(gameContext, id);
		}
	}
}

void SFMLEngine::updateTexture(GameEngine& gameContext, sf::Sprite& node, int id) {
	RenderComponent& drawable = gameContext.entityMan.getComponent<RenderComponent>(id);
	BoundingBox& drawableRect = drawable.spriteRect;

	node.setTextureRect(sf::IntRect(drawableRect.xLeft, drawableRect.yUp, drawableRect.xRight - drawableRect.xLeft, drawableRect.yDown - drawableRect.yUp));
}

void SFMLEngine::updateCamera(GameEngine& gameContext, int id) {
	CameraComponent& cameraComp = gameContext.entityMan.getComponent<CameraComponent>(id);
	SituationComponent& situation = gameContext.entityMan.getComponent<SituationComponent>(id);

	//BoundingBox worldViewRect = Utils::moveToWorldCoords(cameraComp.viewRect, situation);

	cameraMap[id].setSize(cameraComp.viewRect.x * cameraComp.zoom, cameraComp.viewRect.y * cameraComp.zoom);
	cameraMap[id].setCenter(situation.position.x, situation.position.y);
}


void SFMLEngine::createEntity(GameEngine& gameContext, int id) {
	RenderComponent& drawable = gameContext.entityMan.getComponent<RenderComponent>(id);
	sf::Sprite* node = nullptr;

	if (!existsTexture(drawable.sprite)) {
		addTexture(drawable.sprite);
	}


	if (drawable.isHUDElement) {
		HUDNodeMap.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(textureMap[drawable.sprite]));
		node = &HUDNodeMap[id];
	}
	else {
		nodeMap.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(textureMap[drawable.sprite]));
		node = &nodeMap[id];
	}

	if (node != nullptr) {
		// Set the position and data
		updateNode(gameContext, *node, id);
		setColorToEntity(id, drawable.color);
		updateTexture(gameContext, *node, id);
	}
}


void SFMLEngine::createText(GameEngine& gameContext, int id) {
	TextComponent& textComp = gameContext.entityMan.getComponent<TextComponent>(id);

	sf::Text* textNode = nullptr;
	if (textComp.isHUDElement) {
		HUDTextMap.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple());
		textNode = &HUDTextMap[id];
	}

	if (textNode != nullptr) {
		// Set the position and data
		updateText(gameContext, *textNode, id);
	}
}


void SFMLEngine::updateText(GameEngine& gameContext, sf::Text& textNode, int id) {
	SituationComponent& situation = gameContext.entityMan.getComponent<SituationComponent>(id);
	TextComponent& textComp = gameContext.entityMan.getComponent<TextComponent>(id);

	textNode.setPosition(situation.position.x, situation.position.y);
	textNode.setRotation(situation.rotation);
	textNode.setFont(font);
	textNode.setString(textComp.text);
	textNode.setCharacterSize(textComp.size);
	textNode.setFillColor( { textComp.color.r, textComp.color.g, textComp.color.b, textComp.color.a } );
	//text.setStyle(sf::Text::Bold | sf::Text::Underlined);
}


void SFMLEngine::createCamera(GameEngine& gameContext, int id) {
	CameraComponent& cameraComp   = gameContext.entityMan.getComponent<CameraComponent>(id);
	SituationComponent& situation = gameContext.entityMan.getComponent<SituationComponent>(id);

	cameraMap.emplace( std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple() );

	cameraMap[id].setCenter(cameraComp.viewRect.y / 2.f, cameraComp.viewRect.y / 2.f);
	cameraMap[id].setSize(cameraComp.viewRect.y, cameraComp.viewRect.y);
}

void SFMLEngine::eraseEntity(int id) {
	nodeMap.erase(id);
	HUDNodeMap.erase(id);
	cameraMap.erase(id);
	HUDTextMap.erase(id);

	// Delete the texture and image?
}


size_t SFMLEngine::countRenderNodes() const {
	return nodeMap.size();
}

bool SFMLEngine::existsNode(int id) const {
	if (nodeMap.find(id) == nodeMap.end())
		return false;
	return true;
}

bool SFMLEngine::existsHUDNode(int id) const {
	if (HUDNodeMap.find(id) == HUDNodeMap.end())
		return false;
	return true;
}

bool SFMLEngine::existsHUDText(int id) const {
	if (HUDTextMap.find(id) == HUDTextMap.end())
		return false;
	return true;
}

bool SFMLEngine::existsCamera(int id) const {
	if (cameraMap.find(id) == cameraMap.end())
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


void SFMLEngine::setKeyToControl(GameEngine& gameContext, sf::Event& event) const {
	InputComponent& playerInput = gameContext.entityMan.getComponent<InputComponent>(WorldElementsData::playerId);
	bool inputEntered = false;

	std::cout << "key pressed: " << (int)event.key.code << "\n";
	playerInput.keyboardControlsMap[playerInput.controlToChange] = event.key.code;
}
