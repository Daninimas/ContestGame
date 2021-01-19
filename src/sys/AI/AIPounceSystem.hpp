#pragma once

#include "../System.hpp"
#include <com/ComponentsIncluder>

class AIPounceSystem : public System {
public:
    AIPounceSystem();
    ~AIPounceSystem();

    void update(GameEngine& gameContext) const override;

private:
    void stickToObjective(GameEngine& gameContext, AIPounceComponent& pounceComp) const;

    void chaseObjective(GameEngine& gameContext, AIPounceComponent& pounceComp) const;
    void checkStickedToObjective(GameEngine& gameContext, AIPounceComponent& pounceComp) const;
};