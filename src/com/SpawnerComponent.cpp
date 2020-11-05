#include "SpawnerComponent.hpp"

SpawnerComponent::SpawnerComponent(const int id)
    : Component(id), spawnedObjsAlive() {
    spawnedObjsAlive.reserve(10);
}

SpawnerComponent::~SpawnerComponent() {}