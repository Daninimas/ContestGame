#include "AIBombDropSystem.hpp"

#include <eng/GameEngine.hpp>
#include <iostream>

AIBombDropSystem::AIBombDropSystem() {}

AIBombDropSystem::~AIBombDropSystem() {}


void AIBombDropSystem::update(GameEngine& gameContext) const {
	auto& AIBombComponents = gameContext.entityMan->getComponents<AIDropBombComponent>();

	for (AIDropBombComponent& AIDropBombComp : AIBombComponents) {
		AIDropBombComp.createBomb = false;
		AIDropBombComp.cooldown += gameContext.getDeltaTime();

		if (AIDropBombComp.cooldown > AIDropBombComp.maxCooldown) {
			AIDropBombComp.createBomb = true;

			AIDropBombComp.cooldown = 0.f;
		}
	}
}
