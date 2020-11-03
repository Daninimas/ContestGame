#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class DodgeSystem : public System {
public:
    DodgeSystem();
    ~DodgeSystem();

    void update(GameEngine& gameContext) const override;

private:
    void doDodge(GameEngine& gameContext, DodgeComponent& dodgeComp) const;
};