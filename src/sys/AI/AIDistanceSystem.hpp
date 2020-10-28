#pragma once

#include "../System.hpp"
#include <com/ComponentsIncluder>

class AIDistanceSystem : public System {
public:
    AIDistanceSystem();
    ~AIDistanceSystem();

    void update(GameEngine& gameContext) const override;

private:
    void manageAttackGeneration(GameEngine& gameContext, AIDistanceAtkComponent& AImeleeComp) const;
    void setAttackDirection(GameEngine& gameContext, SituationComponent& attackerSit, SituationComponent& objectiveSit, DistanceWeaponComponent& distWeap) const;

};