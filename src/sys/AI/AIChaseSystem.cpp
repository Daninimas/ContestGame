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

	//Do the movement
	if (chaserSit.x < objectiveSit.x) {
		chaserVel.velocityX = chaserVel.speedX;
	}
	else {
		chaserVel.velocityX = -chaserVel.speedX;
	}
}