#pragma once

#include "../System.hpp"
#include <com/ComponentsIncluder>

class AIChaseSystem : public System {
public:
    AIChaseSystem();
    ~AIChaseSystem();

    void update(GameEngine& gameContext) const override;

private:

    void chaseObjective(GameEngine& gameContext, AIChaseComponent& chaseComp) const;
};