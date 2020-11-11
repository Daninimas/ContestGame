#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class FurySystem : public System {
public:
    FurySystem();
    ~FurySystem();

    void update(GameEngine& gameContext) const override;

private:
    void applyFuryToTimers(GameEngine& gameContext, FuryComponent& furyComp) const;
    void setFuryColor(GameEngine& gameContext, FuryComponent& furyComp) const;

};