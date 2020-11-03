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
    uint8_t actualMovement = 0xFF; // start with no movement

    // Reset actions
    playerInput.resetActions();
    playerVel.velocityX = 0.f; // reset the movement

    // Set new actions
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        playerInput.movingUp = true;
        actualMovement = DodgeComponent::Up;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        playerInput.movingDown = true;
        actualMovement = DodgeComponent::Down;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        playerInput.movingLeft = true;
        playerVel.velocityX = -playerVel.speedX;
        actualMovement = DodgeComponent::Left;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        playerInput.movingRight = true;
        playerVel.velocityX = playerVel.speedX;
        actualMovement = DodgeComponent::Right;
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

    activateDodge(gameContext, actualMovement);
}

void InputSystem::activateDodge(GameEngine& gameContext, uint8_t actualMovement) const {
    DodgeComponent& playerDodge = gameContext.entityMan.getComponent<DodgeComponent>(WorldData::playerId);

    playerDodge.timeFromLastMove += gameContext.getDeltaTime();

    if (actualMovement == playerDodge.lastMovemet && playerDodge.timeFromLastMove < playerDodge.dodgeTime && playerDodge.releasedKey) {
        playerDodge.activateDodge = true;
        playerDodge.dodgeDirection = actualMovement;
    }
    
    if (actualMovement != 0xFF) { //if key pressed
        playerDodge.timeFromLastMove = 0.f;
        playerDodge.lastMovemet = actualMovement;
        playerDodge.releasedKey = false;
    }
    else {
        playerDodge.releasedKey = true;
    }
}


void InputSystem::inputMenus(GameEngine& gameContext) const {
    InputComponent& playerInput = gameContext.entityMan.getComponent<InputComponent>(WorldData::playerId);

    playerInput.cooldown += gameContext.getDeltaTime();

    // Reset actions
    playerInput.resetActions();
    playerInput.cooldown += gameContext.getDeltaTime();

    if (playerInput.cooldown > playerInput.maxCooldown) {
        bool buttonPressed = false;

        // Set new actions
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            playerInput.movingUp = true;
            buttonPressed = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            playerInput.movingDown = true;
            buttonPressed = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            playerInput.movingLeft = true;
            buttonPressed = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            playerInput.movingRight = true;
            buttonPressed = true;
        }


        // Reset cooldown
        if (buttonPressed) {
            playerInput.cooldown = 0.f;
        }
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
    {
        playerInput.select = true;
    }
}
