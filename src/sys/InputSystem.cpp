#include "InputSystem.hpp"

#include <eng/GameEngine.hpp>
#include <iostream>

InputSystem::InputSystem() {}

InputSystem::~InputSystem() {}

void InputSystem::update(GameEngine& gameContext) const {

    if (gameContext.getGameState() == GameState::PLAYING) {
        inputPlaying(gameContext);
    }
    else {
        inputMenus(gameContext);
    }
}


void InputSystem::inputPlaying(GameEngine& gameContext) const {
    InputComponent& playerInput = gameContext.entityMan.getComponent<InputComponent>(WorldData::playerId);
    VelocityComponent& playerVel = gameContext.entityMan.getComponent<VelocityComponent>(WorldData::playerId);

    // Reset actions
    playerInput.resetActions();
    playerVel.velocityX = 0.f; // reset the movement

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
        playerVel.velocityX = -playerVel.speedX;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        playerInput.movingRight = true;
        playerVel.velocityX = playerVel.speedX;
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
        auto& jumpComp = gameContext.entityMan.getComponent<JumpComponent>(WorldData::playerId);
        if (jumpComp.cooldow > jumpComp.maxCooldown) { // if has cooldown on floor
            playerVel.velocityY = jumpComp.impulse;
        }

        /*if (jumpComp.jumpIndex == jumpComp.jumptable.size() && jumpComp.cooldow > jumpComp.maxCooldown) { // if has ended jumping && has cooldown on floor
            jumpComp.jumpIndex = 0;
        }*/
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        gameContext.setGameState(GameState::PAUSE);
    }
}


void InputSystem::inputMenus(GameEngine& gameContext) const {
    InputComponent& playerInput = gameContext.entityMan.getComponent<InputComponent>(WorldData::playerId);

    // Reset actions
    playerInput.resetActions();

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
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        playerInput.movingRight = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
    {
        playerInput.select = true;
    }
}
