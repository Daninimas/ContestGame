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
    InputComponent& playerInput = gameContext.entityMan.getComponent<InputComponent>(WorldElementsData::playerId);
    VelocityComponent& playerVel = gameContext.entityMan.getComponent<VelocityComponent>(WorldElementsData::playerId);
    uint8_t actualMovement = 0xFF; // start with no movement

    // Reset actions
    playerInput.resetActions();
    playerVel.velocity.x = 0.f; // reset the movement
    playerInput.movedWithKeyboard = false;

    // Set new actions
    if ( sf::Keyboard::isKeyPressed( static_cast<sf::Keyboard::Key>(playerInput.keyboardControlsMap[Controls::MOVE_UP]) ) )
    {
        playerInput.movingUp = true;
        actualMovement = DodgeComponent::Up;
        playerInput.movedWithKeyboard = true;
    }
    if (sf::Keyboard::isKeyPressed( static_cast<sf::Keyboard::Key>(playerInput.keyboardControlsMap[Controls::MOVE_DOWN]) ))
    {
        playerInput.movingDown = true;
        actualMovement = DodgeComponent::Down;
        playerInput.movedWithKeyboard = true;
    }
    if (sf::Keyboard::isKeyPressed( static_cast<sf::Keyboard::Key>(playerInput.keyboardControlsMap[Controls::MOVE_LEFT]) ))
    {
        playerInput.movingLeft = true;
        playerVel.velocity.x = -playerVel.speedX;
        actualMovement = DodgeComponent::Left;
        playerInput.movedWithKeyboard = true;
    }
    if (sf::Keyboard::isKeyPressed( static_cast<sf::Keyboard::Key>(playerInput.keyboardControlsMap[Controls::MOVE_RIGHT]) ))
    {
        playerInput.movingRight = true;
        playerVel.velocity.x = playerVel.speedX;
        actualMovement = DodgeComponent::Right;
        playerInput.movedWithKeyboard = true;
    }

    if (sf::Keyboard::isKeyPressed( static_cast<sf::Keyboard::Key>(playerInput.keyboardControlsMap[Controls::ACTION]) ))
    {
        playerInput.attacking = true;
        playerInput.movedWithKeyboard = true;
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


    if (sf::Keyboard::isKeyPressed( static_cast<sf::Keyboard::Key>(playerInput.keyboardControlsMap[Controls::JUMP]) ))
    {
        auto& jumpComp = gameContext.entityMan.getComponent<JumpComponent>(WorldElementsData::playerId);
        if (jumpComp.cooldown > jumpComp.maxCooldown) { // if has cooldown on floor
            playerVel.velocity.y = jumpComp.impulse;
        }
        playerInput.movedWithKeyboard = true;

        /*if (jumpComp.jumpIndex == jumpComp.jumptable.size() && jumpComp.cooldow > jumpComp.maxCooldown) { // if has ended jumping && has cooldown on floor
            jumpComp.jumpIndex = 0;
        }*/
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        gameContext.pushGameState(GameState::PAUSE);
    }

    activateDodge(gameContext, actualMovement);
}

void InputSystem::activateDodge(GameEngine& gameContext, uint8_t actualMovement) const {
    DodgeComponent& playerDodge = gameContext.entityMan.getComponent<DodgeComponent>(WorldElementsData::playerId);

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
    InputComponent& playerInput = gameContext.entityMan.getComponent<InputComponent>(WorldElementsData::playerId);

    playerInput.cooldown += gameContext.getDeltaTime();

    // Reset actions
    playerInput.resetActions();
    playerInput.cooldown += gameContext.getDeltaTime();

    if (playerInput.cooldown > playerInput.maxCooldown) {
        bool buttonPressed = false;

        // Set new actions
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            playerInput.movingUp = true;
            buttonPressed = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            playerInput.movingDown = true;
            buttonPressed = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            playerInput.movingLeft = true;
            buttonPressed = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            playerInput.movingRight = true;
            buttonPressed = true;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            playerInput.select = true;
            buttonPressed = true;
        }

        // Reset cooldown
        if (buttonPressed) {
            playerInput.cooldown = 0.f;
        }
    }
}
