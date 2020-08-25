#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class InputSystem : public System {
public:
    InputSystem();
    ~InputSystem();

    void 	update(GameEngine& gameContext) const override;

private:
};