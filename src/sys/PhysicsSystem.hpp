#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class PhysicsSystem : public System {
public:
    PhysicsSystem();
    ~PhysicsSystem();

    void update(GameEngine& gameContext) const override;

private:
    void updateSituations(GameEngine& gameContext) const;
};