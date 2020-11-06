#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class ShieldSystem : public System {
public:
    ShieldSystem();
    ~ShieldSystem();

    void update(GameEngine& gameContext) const override;

private:
    void setInObjectivePosition(GameEngine& gameContext, ShieldComponent& shield) const;

};