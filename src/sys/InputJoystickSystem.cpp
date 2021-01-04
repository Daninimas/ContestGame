#include "InputJoystickSystem.hpp"

#include <eng/GameEngine.hpp>
#include <SFML/Window/Joystick.hpp>
#include <iostream>

#include <tools/AnimationManager.hpp>

//https://code.markrichards.ninja/sfml/working-with-joysticks-in-sfml

const float joystickBias = 25.f;
const float joystickMenuBias = 50.f;

InputJoystickSystem::InputJoystickSystem() {}

InputJoystickSystem::~InputJoystickSystem() {}

void InputJoystickSystem::update(GameEngine& gameContext) const {

    if (sf::Joystick::isConnected(0) && !gameContext.entityMan.getComponent<InputComponent>(WorldElementsData::playerId).movedWithKeyboard) {
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
    AnimationComponent& animComp = gameContext.entityMan.getComponent<AnimationComponent>(WorldElementsData::playerId);

    //check state of joystick
    Vector2 joystickPos = Vector2(sf::Joystick::getAxisPosition(0, sf::Joystick::X), sf::Joystick::getAxisPosition(0, sf::Joystick::Y));  // in range [-100 .. 100]

    //CHECK DEAD ZONES - OTHERWISE INPUT WILL RESULT IN JITTERY MOVEMENTS WHEN NO INPUT IS PROVIDED
    //INPUT RANGES FROM -100 TO 100
    //A 15% DEAD ZONE SEEMS TO WORK FOR ME - GIVE THAT A SHOT

    if (joystickPos.y < -joystickBias)
    {
        playerInput.movingUp = true;
        playerInput.actualMovement = DodgeComponent::Up;
    }
    if (joystickPos.y > joystickBias)
    {
        playerInput.movingDown = true;
        playerInput.actualMovement = DodgeComponent::Down;
    }
    if (joystickPos.x < -joystickBias)
    {
        playerInput.movingLeft = true;
        playerVel.velocity.x = playerVel.speedX * (joystickPos.x / 100.f);
        playerInput.actualMovement = DodgeComponent::Left;
        AnimationManager::setAnimationToEntity(gameContext, Animation::RUNNING, animComp);
    }
    if (joystickPos.x > joystickBias)
    {
        playerInput.movingRight = true;
        playerVel.velocity.x = playerVel.speedX * (joystickPos.x / 100.f);
        playerInput.actualMovement = DodgeComponent::Right;
        AnimationManager::setAnimationToEntity(gameContext, Animation::RUNNING, animComp);
    }

    if (sf::Joystick::isButtonPressed(0, playerInput.keyboardControlsMap[Controls::JOYSTICK_ACTION])) //"B" button on the XBox 360 controller
    {
        playerInput.attacking = true;
    }


    if (sf::Joystick::isButtonPressed(0, playerInput.keyboardControlsMap[Controls::JOYSTICK_JUMP])) //"A" button on the XBox 360 controller
    {
        auto& jumpComp = gameContext.entityMan.getComponent<JumpComponent>(WorldElementsData::playerId);
        if (jumpComp.cooldown > jumpComp.maxCooldown) { // if has cooldown on floor
            playerVel.velocity.y = jumpComp.impulse;
        }
    }

    // Set idle animation if not moved
    if (playerInput.actualMovement == 0xFF) {
        AnimationManager::setAnimationToEntity(gameContext, Animation::IDLE, animComp);
    }
}



void InputJoystickSystem::inputMenus(GameEngine& gameContext) const {
    InputComponent& playerInput = gameContext.entityMan.getComponent<InputComponent>(WorldElementsData::playerId);

    if (playerInput.cooldown > playerInput.maxCooldown) {
        //check state of joystick
        Vector2 joystickPos = Vector2(sf::Joystick::getAxisPosition(0, sf::Joystick::X), sf::Joystick::getAxisPosition(0, sf::Joystick::Y));  // in range [-100 .. 100]

        bool buttonPressed = false;

        if (joystickPos.y < -joystickMenuBias)
        {
            playerInput.movingUp = true;
            buttonPressed = true;
        }
        if (joystickPos.y > joystickMenuBias)
        {
            playerInput.movingDown = true;
            buttonPressed = true;
        }
        if (joystickPos.x < -joystickMenuBias)
        {
            playerInput.movingLeft = true;
            buttonPressed = true;
        }
        if (joystickPos.x > joystickMenuBias)
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
