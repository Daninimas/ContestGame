#pragma once

#include "../System.hpp"
#include <com/ComponentsIncluder>

class AIFlyingChaseSystem : public System {
public:
    AIFlyingChaseSystem();
    ~AIFlyingChaseSystem();

    void update(GameEngine& gameContext) const override;

private:

    bool chaseObjective(GameEngine& gameContext, AIFlyingChaseComponent& chaseComp) const;
    //void doIdleAnimation(GameEngine& gameContext, AIFlyingChaseComponent& chaseComp) const;
};