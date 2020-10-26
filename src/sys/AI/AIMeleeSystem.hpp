#pragma once

#include "../System.hpp"
#include <com/ComponentsIncluder>

class AIMeleeSystem : public System {
public:
    AIMeleeSystem();
    ~AIMeleeSystem();

    void update(GameEngine& gameContext) const override;

private:
    void manageAttacksGeneration(GameEngine& gameContext, AIMeleeComponent& AImeleeComp) const;
};