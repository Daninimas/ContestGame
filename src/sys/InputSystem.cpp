#include "InputSystem.hpp"

#include <eng/GameEngine.hpp>
#include <iostream>

InputSystem::InputSystem() {}

InputSystem::~InputSystem() {}

void InputSystem::update(GameEngine& gameContext) const {
    InputComponent    &playerInput = gameContext.entityMan.getComponent<InputComponent>(gameContext.playerId);
    VelocityComponent &playerVel   = gameContext.entityMan.getComponent<VelocityComponent>(gameContext.playerId);
    bool moved = false;

    // Reset actions
    playerInput.resetActions();
    playerVel.resetAllVelocities();

    // Set new actions
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        playerInput.movingUp = true;
        moved = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        playerInput.movingDown = true;
        moved = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        playerInput.movingLeft = true;
        playerVel.velocityX = -playerVel.speed;
        moved = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        playerInput.movingRight = true;
        playerVel.velocityX = playerVel.speed;
        moved = true;
    }

    // Mark to update in physics system
    if (moved) {
        gameContext.entityMan.addEntityToUpdate(gameContext.playerId);
    }
}
