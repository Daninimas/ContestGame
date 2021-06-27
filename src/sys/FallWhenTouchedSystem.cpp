#include "FallWhenTouchedSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>
#include <iostream>

FallWhenTouchedSystem::FallWhenTouchedSystem() {}

FallWhenTouchedSystem::~FallWhenTouchedSystem() {}

void FallWhenTouchedSystem::update(GameEngine& gameContext) const {
	auto& fallComponents = gameContext.entityMan->getComponents<FallWhenTouchedComponent>();
	std::vector<int> fallCompsToDelete;
	fallCompsToDelete.reserve(fallComponents.size());

	for (FallWhenTouchedComponent& fallComp : fallComponents) {
		ColliderComponent& collComp = gameContext.entityMan->getComponent<ColliderComponent>(fallComp.id);

		if (!fallComp.activated) {
			if (Utils::checkCollidingWithEntity(collComp.boundingRoot, fallComp.objectiveId)) {
				fallComp.activated = true;
			}
		}
		else {
			fallComp.countdown += gameContext.getDeltaTime();

			if (fallComp.countdown >= fallComp.maxCountdown) {
				setGravity(gameContext, fallComp);
				fallCompsToDelete.emplace_back(fallComp.id);
			}
		}
	}


	for (int id : fallCompsToDelete) {
		gameContext.entityMan->eraseComponent<FallWhenTouchedComponent>(id);
	}
}


void FallWhenTouchedSystem::setGravity(GameEngine& gameContext, FallWhenTouchedComponent& fallComp) const {
	// Create Velocity component and set gravity
	VelocityComponent& velocityComp = gameContext.entityMan->createComponent<VelocityComponent>(fallComp.id);

	velocityComp.gravity = fallComp.gravity;
	velocityComp.speedX = 0.f;
}
