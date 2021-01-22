#pragma once

#include "../System.hpp"
#include <com/ComponentsIncluder>

class AIDroneSystem : public System {
public:
    AIDroneSystem();
    ~AIDroneSystem();

    void update(GameEngine& gameContext) const override;

private:
    bool objectiveInsideSensor(GameEngine& gameContext, AIDistanceAtkComponent& distanceAIComp) const;
    void setNewObjective(GameEngine& gameContext, AIDistanceAtkComponent& distanceAIComp) const;
};