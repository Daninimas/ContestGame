#pragma once

#include "System.hpp"
#include <SFML/Window/Joystick.hpp>
#include <com/ComponentsIncluder>

class InputJoystickSystem : public System {
public:
    InputJoystickSystem();
    ~InputJoystickSystem();

    void 	update(GameEngine& gameContext) const override;

private:
    void inputPlaying(GameEngine& gameContext) const;
    void inputMenus(GameEngine& gameContext) const;

};