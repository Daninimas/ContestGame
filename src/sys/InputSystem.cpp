#include "InputSystem.hpp"

#include <eng/GameEngine.hpp>
#include <iostream>

InputSystem::InputSystem() {}

InputSystem::~InputSystem() {}

void InputSystem::update(GameEngine& gameContext) const {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        std::cout << "Izquierda\n";
    }
}
