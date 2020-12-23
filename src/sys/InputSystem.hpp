#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class InputSystem : public System {
public:
    InputSystem();
    ~InputSystem();

    void 	update(GameEngine& gameContext) const override;

private:
    void inputPlaying(GameEngine& gameContext) const;
    void inputMenus(GameEngine& gameContext) const;
};