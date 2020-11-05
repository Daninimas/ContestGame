#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class SpawnSystem : public System {
public:
    SpawnSystem();
    ~SpawnSystem();

    void update(GameEngine& gameContext) const override;

private:
    void manageSpawn(GameEngine& gameContext, SpawnerComponent& spawnComp) const;
    void spawnObject(GameEngine& gameContext, SpawnerComponent& spawnComp, SituationComponent& spawnSit) const;
    void setobjectiveToAIComponents(GameEngine& gameContext, int newEntityID, int objectiveId) const;

};