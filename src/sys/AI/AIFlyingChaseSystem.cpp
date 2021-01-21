#include "AIFlyingChaseSystem.hpp"

#include <eng/GameEngine.hpp>
#include <iostream>

AIFlyingChaseSystem::AIFlyingChaseSystem() {}

AIFlyingChaseSystem::~AIFlyingChaseSystem() {}


void AIFlyingChaseSystem::update(GameEngine& gameContext) const {

	auto& chaseComponents = gameContext.entityMan.getComponents<AIFlyingChaseComponent>();

	for (AIFlyingChaseComponent& chaseComp : chaseComponents)
	{
		if (gameContext.entityMan.existsComponent<SituationComponent>(chaseComp.objectiveId)) {
			bool idle = chaseObjective(gameContext, chaseComp);

			/*
			if (idle) {
				doIdleAnimation(gameContext, chaseComp);
			}
			else {
				chaseComp.animationMovementTimeCounter = 0.f;
			}
			*/
		}
	}
}


bool AIFlyingChaseSystem::chaseObjective(GameEngine& gameContext, AIFlyingChaseComponent& chaseComp) const {  // Returns true if the entity is IDLE
	SituationComponent& objectiveSit = gameContext.entityMan.getComponent<SituationComponent>(chaseComp.objectiveId);
	SituationComponent& chaserSit = gameContext.entityMan.getComponent<SituationComponent>(chaseComp.id);
	VelocityComponent& chaserVel = gameContext.entityMan.getComponent<VelocityComponent>(chaseComp.id);
	bool idle = false;

	if (abs(chaserSit.position.x - objectiveSit.position.x) > chaseComp.minDistanceX) {
		//Do the movement
		if (chaserSit.position.x < objectiveSit.position.x) {
			chaserVel.velocity.x = chaserVel.speedX;
			idle = false;
		}
		else {
			chaserVel.velocity.x = -chaserVel.speedX;
		}
	}
	else {
		chaserVel.velocity.x = 0.f;
		idle = true;
	}

	
	if ((objectiveSit.position.y - chaserSit.position.y) < chaseComp.minHeigtht) {
		chaserVel.velocity.y = -chaserVel.speedX;
		idle = false;
	}
	else if ((objectiveSit.position.y - chaserSit.position.y) > chaseComp.maxHeigtht) {
		chaserVel.velocity.y = chaserVel.speedX;
		idle = false;
	}
	else {
		chaserVel.velocity.y = 0.f;
		idle = true;
	}

	return idle;
}

/*
void AIFlyingChaseSystem::doIdleAnimation(GameEngine& gameContext, AIFlyingChaseComponent& chaseComp) const {
	// Move up and down the entity a little doing an animation
	VelocityComponent& chaserVel = gameContext.entityMan.getComponent<VelocityComponent>(chaseComp.id);
}
*/