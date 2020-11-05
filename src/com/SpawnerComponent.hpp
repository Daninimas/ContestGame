#pragma once
#include "Component.hpp"

#include <enum/GameObjectType.hpp>
#include <enum/EntityType.hpp>
#include <limits>
#include <vector>

class SpawnerComponent : public Component {
public:
    explicit SpawnerComponent(const int id);
    ~SpawnerComponent() override;

    GameObjectType spawnObjectsType;
    EntityType spawnEntitiesType;

    float cooldown{ 0.f };
    float maxCooldown{ 5.f };

    int objectiveId;

    // if we want the spawner to activate in a certain to the objective
    float rangeX{ 300.f };
    float rangeY{ 300.f };

    // if we want the spawner to spawn a certain number ob objects and die
    uint16_t numObjectsToSpawn{ std::numeric_limits<uint16_t>::max() }; // if we want it to be unlimited, with this number it is ok

    uint8_t maxSpawnedObjsAlive{ 5 }; // the number of object that are in the world simustaneously (for optimization)

    std::vector<int> spawnedObjsAlive;
};
