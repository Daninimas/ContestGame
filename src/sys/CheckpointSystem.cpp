#include "CheckpointSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>


CheckpointSystem::CheckpointSystem() {}

CheckpointSystem::~CheckpointSystem() {}


void CheckpointSystem::update(GameEngine& gameContext) const {
	auto& checkpointComponents = gameContext.entityMan->getComponents<CheckpointComponent>();

	for (CheckpointComponent& checkpoint : checkpointComponents) {
		if (!checkpoint.active) {
			checkActivation(gameContext, checkpoint);
		}
	}
}

void CheckpointSystem::checkActivation(GameEngine& gameContext, CheckpointComponent& checkpoint) const {
	ColliderComponent& collider = gameContext.entityMan->getComponent<ColliderComponent>(checkpoint.id);

	checkpoint.active = Utils::checkCollidingWithEntity(collider.boundingRoot, WorldElementsData::playerId); // Si colisiona con el jugador se activa
}