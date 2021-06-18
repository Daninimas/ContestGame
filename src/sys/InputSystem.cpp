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
    InputComponent& playerInput  = gameContext.entityMan->getComponent<InputComponent>(WorldElementsData::playerId);
    VelocityComponent& playerVel = gameContext.entityMan->getComponent<VelocityComponent>(WorldElementsData::playerId);

    // Reset actions
    playerInput.resetActions();
    playerVel.velocity.x = 0.f; // reset the movement
    playerInput.movedWithKeyboard = false;

    // Set new actions
    if ( sf::Keyboard::isKeyPressed( static_cast<sf::Keyboard::Key>(playerInput.keyboardControlsMap[Controls::MOVE_UP]) ) )
    {
        playerInput.movingUp = true;
        playerInput.actualMovement = DodgeComponent::Up;
        playerInput.movedWithKeyboard = true;
    }
    if (sf::Keyboard::isKeyPressed( static_cast<sf::Keyboard::Key>(playerInput.keyboardControlsMap[Controls::MOVE_DOWN]) ))
    {
        playerInput.movingDown = true;
        playerInput.actualMovement = DodgeComponent::Down;
        playerInput.movedWithKeyboard = true;
    }
    if (sf::Keyboard::isKeyPressed( static_cast<sf::Keyboard::Key>(playerInput.keyboardControlsMap[Controls::MOVE_LEFT]) ))
    {
        playerInput.movingLeft = true;
        if (!playerInput.usingTurret)
            playerVel.velocity.x = -playerVel.speedX;
        playerInput.actualMovement = DodgeComponent::Left;
        playerInput.movedWithKeyboard = true;
    }
    if (sf::Keyboard::isKeyPressed( static_cast<sf::Keyboard::Key>(playerInput.keyboardControlsMap[Controls::MOVE_RIGHT]) ))
    {
        playerInput.movingRight = true;
        if (!playerInput.usingTurret)
            playerVel.velocity.x = playerVel.speedX;
        playerInput.actualMovement = DodgeComponent::Right;
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
        auto& jumpComp = gameContext.entityMan->getComponent<JumpComponent>(WorldElementsData::playerId);
        if (jumpComp.cooldown > jumpComp.maxCooldown) { // if has cooldown on floor
            playerVel.velocity.y = jumpComp.impulse;

            // Play jump sound
            gameContext.getSoundFacadeRef().loadSound(jumpComp.jumpSound.soundPath);
            gameContext.getSoundFacadeRef().playSound(jumpComp.jumpSound);
        }
        playerInput.movedWithKeyboard = true;
        playerInput.jumping = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        gameContext.pushGameState(GameState::PAUSE);
    }
}


void InputSystem::inputMenus(GameEngine& gameContext) const {
    InputComponent& playerInput = gameContext.entityMan->getComponent<InputComponent>(WorldElementsData::playerId);

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
