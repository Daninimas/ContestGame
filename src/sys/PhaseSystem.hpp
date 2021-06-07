#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class PhaseSystem : public System {
public:
    PhaseSystem();
    ~PhaseSystem();

    void update(GameEngine& gameContext) const override;

private:
    void checkIfChangePhase(GameEngine& gameContext) const;

    void changeToNextPhase (GameEngine& gameContext, WorldComponent& worldComp) const;

    void setPhaseTimePunctuation(WorldPhase& phase) const;
};