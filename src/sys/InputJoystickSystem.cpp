#include "InputJoystickSystem.hpp"

#include <eng/GameEngine.hpp>
#include <iostream>

//https://code.markrichards.ninja/sfml/working-with-joysticks-in-sfml

InputJoystickSystem::InputJoystickSystem() {}

InputJoystickSystem::~InputJoystickSystem() {}

void InputJoystickSystem::update(GameEngine& gameContext) const {

    if (sf::Joystick::isConnected(0)) {
        if (gameContext.getGameState() == GameState::PLAYING) {
            inputPlaying(gameContext);
        }
        else {
            inputMenus(gameContext);
        }
    }
}


void InputJoystickSystem::inputPlaying(GameEngine& gameContext) const {
    InputComponent& playerInput = gameContext.entityMan.getComponent<InputComponent>(WorldElementsData::playerId);
    VelocityComponent& playerVel = gameContext.entityMan.getComponent<VelocityComponent>(WorldElementsData::playerId);
    uint8_t actualMovement = 0xFF; // start with no movement

    //check state of joystick
    Vector2 joystickPos = Vector2(sf::Joystick::getAxisPosition(0, sf::Joystick::X), sf::Joystick::getAxisPosition(0, sf::Joystick::Y));  // in range [-100 .. 100]

    //CHECK DEAD ZONES - OTHERWISE INPUT WILL RESULT IN JITTERY MOVEMENTS WHEN NO INPUT IS PROVIDED
    //INPUT RANGES FROM -100 TO 100
    //A 15% DEAD ZONE SEEMS TO WORK FOR ME - GIVE THAT A SHOT
    if (joystickPos.x > 15.f || joystickPos.x < -15.f || joystickPos.y > 15.f || joystickPos.y < -15.f) {
        // Set new actions
        if (joystickPos.y < 0.f)
        {
            playerInput.movingUp = true;
            actualMovement = DodgeComponent::Up;
        }
        if (joystickPos.y > 0.f)
        {
            playerInput.movingDown = true;
            actualMovement = DodgeComponent::Down;
        }
        if (joystickPos.x < 0.f)
        {
            playerInput.movingLeft = true;
            playerVel.velocity.x = playerVel.speedX * (joystickPos.x / 100.f);
            actualMovement = DodgeComponent::Left;
        }
        if (joystickPos.x > 0.f)
        {
            playerInput.movingRight = true;
            playerVel.velocity.x = playerVel.speedX * (joystickPos.x / 100.f);
            actualMovement = DodgeComponent::Right;
        }
    }

    if (sf::Joystick::isButtonPressed(0, 1)) //"B" button on the XBox 360 controller
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


    if (sf::Joystick::isButtonPressed(0, 0)) //"A" button on the XBox 360 controller
    {
        auto& jumpComp = gameContext.entityMan.getComponent<JumpComponent>(WorldElementsData::playerId);
        if (jumpComp.cooldown > jumpComp.maxCooldown) { // if has cooldown on floor
            playerVel.velocity.y = jumpComp.impulse;
        }

        /*if (jumpComp.jumpIndex == jumpComp.jumptable.size() && jumpComp.cooldow > jumpComp.maxCooldown) { // if has ended jumping && has cooldown on floor
            jumpComp.jumpIndex = 0;
        }*/
    }

    activateDodge(gameContext, actualMovement);
}

void InputJoystickSystem::activateDodge(GameEngine& gameContext, uint8_t actualMovement) const {
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


void InputJoystickSystem::inputMenus(GameEngine& gameContext) const {
    InputComponent& playerInput = gameContext.entityMan.getComponent<InputComponent>(WorldElementsData::playerId);

    playerInput.cooldown += gameContext.getDeltaTime();

    // Reset actions
    playerInput.resetActions();
    playerInput.cooldown += gameContext.getDeltaTime();

    if (playerInput.cooldown > playerInput.maxCooldown) {
        //check state of joystick
        Vector2 joystickPos = Vector2(sf::Joystick::getAxisPosition(0, sf::Joystick::X), sf::Joystick::getAxisPosition(0, sf::Joystick::Y));  // in range [-100 .. 100]

        //CHECK DEAD ZONES - OTHERWISE INPUT WILL RESULT IN JITTERY MOVEMENTS WHEN NO INPUT IS PROVIDED
        //INPUT RANGES FROM -100 TO 100
        //A 15% DEAD ZONE SEEMS TO WORK FOR ME - GIVE THAT A SHOT
        if (joystickPos.x > 15.f || joystickPos.x < -15.f || joystickPos.y > 15.f || joystickPos.y < -15.f) {

            bool buttonPressed = false;

            // Set new actions
            if (joystickPos.y < 0.f)
            {
                playerInput.movingUp = true;
                buttonPressed = true;
            }
            if (joystickPos.y > 0.f)
            {
                playerInput.movingDown = true;
                buttonPressed = true;
            }
            if (joystickPos.x < 0.f)
            {
                playerInput.movingLeft = true;
                buttonPressed = true;
            }
            if (joystickPos.x > 0.f)
            {
                playerInput.movingRight = true;
                buttonPressed = true;
            }

            if (sf::Joystick::isButtonPressed(0, 0))
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
}
