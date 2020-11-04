#include "AIPounceSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>
#include <iostream>

AIPounceSystem::AIPounceSystem() {}

AIPounceSystem::~AIPounceSystem() {}


void AIPounceSystem::update(GameEngine& gameContext) const {
	auto& pounceComponents = gameContext.entityMan.getComponents<AIPounceComponent>();

	for (AIPounceComponent& pounceComp : pounceComponents)
	{
		VelocityComponent& velComp = gameContext.entityMan.getComponent<VelocityComponent>(pounceComp.id);

		if (velComp.velocityY == 0.f) { // if on ground
			chaseObjective(gameContext, pounceComp);
		}
	}
}


void AIPounceSystem::chaseObjective(GameEngine& gameContext, AIPounceComponent& pounceComp) const {
	SituationComponent& objectiveSit = gameContext.entityMan.getComponent<SituationComponent>(pounceComp.objectiveId);
	SituationComponent& chaserSit    = gameContext.entityMan.getComponent<SituationComponent>(pounceComp.id);
	VelocityComponent& chaserVel     = gameContext.entityMan.getComponent<VelocityComponent>(pounceComp.id);
	JumpComponent& pouncerJump       = gameContext.entityMan.getComponent<JumpComponent>(pounceComp.id);


	// Update cooldown
	pounceComp.cooldown += gameContext.getDeltaTime();

	//Do the movement
	if (chaserSit.x < objectiveSit.x) {
		chaserVel.velocityX = chaserVel.speedX;
	}
	else {
		chaserVel.velocityX = -chaserVel.speedX;
	}

	if (!Utils::objectiveInsideRange(chaserSit, objectiveSit, pounceComp.rangeX, pounceComp.rangeY) || pounceComp.cooldown <= pounceComp.maxCooldown) { // if objective not in pounce range, chase
		// For jumping obstacles
		SensorComponent& chaserSens = gameContext.entityMan.getComponent<SensorComponent>(pounceComp.id);
		// Search if colliding with Wall and jump to advance
		for (int sensoredEnt : chaserSens.entitiesSensoring) {
			if (gameContext.entityMan.getEntity(sensoredEnt).getType() == EntityType::WALL) {
				// Jump
				if (pouncerJump.cooldown > pouncerJump.maxCooldown) { // if has cooldown on floor
					chaserVel.velocityY = pouncerJump.impulse;
				}
			}
		}
	}
	else {
		chaserVel.velocityX *= pounceComp.velocityIncFactor;

		if (pouncerJump.cooldown > pouncerJump.maxCooldown) { // if has cooldown on floor
			chaserVel.velocityY = pouncerJump.impulse;
		}

		pounceComp.cooldown = 0.f;
	}
}