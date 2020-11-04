#include "AIChaseSystem.hpp"

#include <eng/GameEngine.hpp>
#include <iostream>

AIChaseSystem::AIChaseSystem() {}

AIChaseSystem::~AIChaseSystem() {}


void AIChaseSystem::update(GameEngine& gameContext) const {

	auto& chaseComponents = gameContext.entityMan.getComponents<AIChaseComponent>();

	for (AIChaseComponent& chaseComp : chaseComponents)
	{
		if (gameContext.entityMan.existsComponent<SituationComponent>(chaseComp.objectiveId)) {
			chaseObjective(gameContext, chaseComp);
		}
	}
}


void AIChaseSystem::chaseObjective(GameEngine& gameContext, AIChaseComponent& chaseComp) const {
	SituationComponent& objectiveSit = gameContext.entityMan.getComponent<SituationComponent>(chaseComp.objectiveId);
	SituationComponent& chaserSit    = gameContext.entityMan.getComponent<SituationComponent>(chaseComp.id);
	VelocityComponent& chaserVel     = gameContext.entityMan.getComponent<VelocityComponent>(chaseComp.id);

	if (abs(chaserSit.x - objectiveSit.x) > chaseComp.minDistanceX) {
		//Do the movement
		if (chaserSit.x < objectiveSit.x) {
			chaserVel.velocityX = chaserVel.speedX;
		}
		else {
			chaserVel.velocityX = -chaserVel.speedX;
		}
	}
	else {
		chaserVel.velocityX = 0.f;
	}

	// For the CHASERJUMPERS
	if (gameContext.entityMan.existsComponent<JumpComponent>(chaseComp.id)) {
		JumpComponent& chaserJump   = gameContext.entityMan.getComponent<JumpComponent>(chaseComp.id);
		SensorComponent& chaserSens = gameContext.entityMan.getComponent<SensorComponent>(chaseComp.id);

		// Search if colliding with Wall and jump to advance
		for (int sensoredEnt : chaserSens.entitiesSensoring) {
			if (gameContext.entityMan.getEntity(sensoredEnt).getType() == EntityType::WALL) {
				// Jump
				if (chaserJump.cooldown > chaserJump.maxCooldown) { // if has cooldown on floor
					chaserVel.velocityY = chaserJump.impulse;
				}
			}
		}
	}
}