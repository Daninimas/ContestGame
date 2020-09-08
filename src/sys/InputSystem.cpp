#include "InputSystem.hpp"

#include <eng/GameEngine.hpp>
#include <iostream>

InputSystem::InputSystem() {}

InputSystem::~InputSystem() {}

void InputSystem::update(GameEngine& gameContext) const {
    InputComponent    &playerInput = gameContext.entityMan.getComponent<InputComponent>(gameContext.playerId);
    VelocityComponent &playerVel   = gameContext.entityMan.getComponent<VelocityComponent>(gameContext.playerId);

    // Reset actions
    playerInput.resetActions();
    playerVel.resetAllVelocities();

    // Set new actions
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        playerInput.movingUp = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        playerInput.movingDown = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        playerInput.movingLeft = true;
        playerVel.velocityX = -playerVel.speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        playerInput.movingRight = true;
        playerVel.velocityX = playerVel.speed;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
    {
        playerInput.attacking = true;
    }
    /*if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
    {
        // This is obligates the player to release the key and press again
        if (playerInput.canAttack) {
            playerInput.attacking = true;
            playerInput.canAttack = false;
        }
    }
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
        playerInput.canAttack = true;
    }*/


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
    {
        playerInput.jumping = true;
    }
}
