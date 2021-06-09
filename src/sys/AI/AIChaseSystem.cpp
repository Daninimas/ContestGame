#include "AIChaseSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>
#include <iostream>

AIChaseSystem::AIChaseSystem() {}

AIChaseSystem::~AIChaseSystem() {}


void AIChaseSystem::update(GameEngine& gameContext) const {

	auto& chaseComponents = gameContext.entityMan->getComponents<AIChaseComponent>();

	for (AIChaseComponent& chaseComp : chaseComponents)
	{
		if (gameContext.entityMan->existsComponent<SituationComponent>(chaseComp.objectiveId)) {
			chaseObjective(gameContext, chaseComp);
		}
	}
}


void AIChaseSystem::chaseObjective(GameEngine& gameContext, AIChaseComponent& chaseComp) const {
	SituationComponent& objectiveSit = gameContext.entityMan->getComponent<SituationComponent>(chaseComp.objectiveId);
	SituationComponent& chaserSit    = gameContext.entityMan->getComponent<SituationComponent>(chaseComp.id);
	VelocityComponent& chaserVel     = gameContext.entityMan->getComponent<VelocityComponent>(chaseComp.id);

	// Objetive center
	float objetiveCenterPositionX = objectiveSit.position.x;
	if (gameContext.entityMan->existsComponent<ColliderComponent>(chaseComp.objectiveId)) {
		objetiveCenterPositionX += Utils::getCenterOfBounding(gameContext.entityMan->getComponent<ColliderComponent>(chaseComp.objectiveId).boundingRoot.bounding).x;
	}
	// Chaser center
	float chaserCenterPositionX = chaserSit.position.x;
	if (gameContext.entityMan->existsComponent<ColliderComponent>(chaseComp.objectiveId)) {
		chaserCenterPositionX += Utils::getCenterOfBounding(gameContext.entityMan->getComponent<ColliderComponent>(chaseComp.id).boundingRoot.bounding).x;
	}

	if (abs(chaserCenterPositionX - objetiveCenterPositionX) > chaseComp.minDistanceX) {
		//Do the movement
		if (chaserSit.position.x < objectiveSit.position.x) {
			chaserVel.velocity.x = chaserVel.speedX;
		}
		else {
			chaserVel.velocity.x = -chaserVel.speedX;
		}
	}
	else {
		chaserVel.velocity.x = 0.f;
	}

	// For the CHASERJUMPERS
	if (gameContext.entityMan->existsComponent<JumpComponent>(chaseComp.id)) {
		JumpComponent& chaserJump   = gameContext.entityMan->getComponent<JumpComponent>(chaseComp.id);
		SensorComponent& chaserSens = gameContext.entityMan->getComponent<SensorComponent>(chaseComp.id);

		// Search if colliding with Wall and jump to advance
		for (int sensoredEnt : chaserSens.entitiesSensoring) {
			if (gameContext.entityMan->getEntity(sensoredEnt).getType() == EntityType::WALL) {
				// Jump
				if (chaserJump.cooldown > chaserJump.maxCooldown) { // if has cooldown on floor
					chaserVel.velocity.y = chaserJump.impulse;
				}
			}
		}
	}
}