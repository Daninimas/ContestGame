#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class InputJoystickSystem : public System {
public:
    InputJoystickSystem();
    ~InputJoystickSystem();

    void 	update(GameEngine& gameContext) const override;

private:
    void inputPlaying(GameEngine& gameContext) const;
    void inputMenus(GameEngine& gameContext) const;

    void setAnimationToPlayer(GameEngine& gameContext) const;
};