#include "AIPounceSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>
#include <iostream>

AIPounceSystem::AIPounceSystem() {}

AIPounceSystem::~AIPounceSystem() {}


void AIPounceSystem::update(GameEngine& gameContext) const {
	auto& pounceComponents = gameContext.entityMan->getComponents<AIPounceComponent>();

	for (AIPounceComponent& pounceComp : pounceComponents)
	{
		if (pounceComp.sticked) {
			stickToObjective(gameContext, pounceComp);
		}
		else {

			VelocityComponent& velComp = gameContext.entityMan->getComponent<VelocityComponent>(pounceComp.id);

			if (velComp.velocity.y == 0.f) { // if on ground
				chaseObjective(gameContext, pounceComp);
			}
			else {
				// To activate the be sticked to objective behavior
				if (pounceComp.isStickyPouncer) {
					checkStickedToObjective(gameContext, pounceComp);
				}
			}
		}
	}
}


void AIPounceSystem::chaseObjective(GameEngine& gameContext, AIPounceComponent& pounceComp) const {
	SituationComponent& objectiveSit = gameContext.entityMan->getComponent<SituationComponent>(pounceComp.objectiveId);
	SituationComponent& chaserSit    = gameContext.entityMan->getComponent<SituationComponent>(pounceComp.id);
	VelocityComponent& chaserVel     = gameContext.entityMan->getComponent<VelocityComponent>(pounceComp.id);
	JumpComponent& pouncerJump       = gameContext.entityMan->getComponent<JumpComponent>(pounceComp.id);


	// Update cooldown
	pounceComp.cooldown += gameContext.getDeltaTime();

	//Do the movement
	if (chaserSit.position.x < objectiveSit.position.x) {
		chaserVel.velocity.x = chaserVel.speedX;
	}
	else {
		chaserVel.velocity.x = -chaserVel.speedX;
	}

	if (!Utils::objectiveInsideRange(chaserSit, objectiveSit, pounceComp.range) || pounceComp.cooldown <= pounceComp.maxCooldown) { // if objective not in pounce range, chase
		// For jumping obstacles
		SensorComponent& chaserSens = gameContext.entityMan->getComponent<SensorComponent>(pounceComp.id);
		// Search if colliding with Wall and jump to advance
		for (int sensoredEnt : chaserSens.entitiesSensoring) {
			if (gameContext.entityMan->getEntity(sensoredEnt).getType() == EntityType::WALL) {
				// Jump
				if (pouncerJump.cooldown > pouncerJump.maxCooldown) { // if has cooldown on floor
					chaserVel.velocity.y = pouncerJump.impulse;
				}
			}
		}
	}
	else {
		chaserVel.velocity.x *= pounceComp.velocityIncFactor;

		if (pouncerJump.cooldown > pouncerJump.maxCooldown) { // if has cooldown on floor
			chaserVel.velocity.y = pouncerJump.impulse;

			// Play jump sound
			gameContext.getSoundFacadeRef().loadSound(pouncerJump.jumpSound.soundPath);
			gameContext.getSoundFacadeRef().playSound(pouncerJump.jumpSound);
		}

		pounceComp.cooldown = 0.f;
	}
}


void AIPounceSystem::checkStickedToObjective(GameEngine& gameContext, AIPounceComponent& pounceComp) const {
	ColliderComponent& colliderComp = gameContext.entityMan->getComponent<ColliderComponent>(pounceComp.id);

	if (Utils::checkCollidingWithEntity(colliderComp.boundingRoot, pounceComp.objectiveId)) {
		pounceComp.sticked = true;		
	}
}



void AIPounceSystem::stickToObjective(GameEngine& gameContext, AIPounceComponent& pounceComp) const {
	SituationComponent& objectiveSit = gameContext.entityMan->getComponent<SituationComponent>(pounceComp.objectiveId);
	SituationComponent& pouncerSit   = gameContext.entityMan->getComponent<SituationComponent>(pounceComp.id);

	// Si está muy alejado de su objetivo, se separa
	Vector2 distance = { abs(pouncerSit.position.x - objectiveSit.position.x), abs(pouncerSit.position.y - objectiveSit.position.y) };
	if (distance.x > 700.f || distance.y > 700.f) {
		pounceComp.sticked = false;
		return;
	}

	if (objectiveSit.facing == SituationComponent::Left) {
		ColliderComponent& pouncerColl = gameContext.entityMan->getComponent<ColliderComponent>(pounceComp.id);

		pouncerSit.facing = SituationComponent::Right;
		pouncerSit.position = objectiveSit.position;
		pouncerSit.position.x -= pouncerColl.boundingRoot.bounding.xRight - pouncerColl.boundingRoot.bounding.xLeft;
	}
	else {
		ColliderComponent& objectiveColl = gameContext.entityMan->getComponent<ColliderComponent>(pounceComp.objectiveId);

		pouncerSit.facing = SituationComponent::Left;
		pouncerSit.position = objectiveSit.position;
		pouncerSit.position.x += objectiveColl.boundingRoot.bounding.xRight + 0.1f;
	}

	// Set the x velocity to 0
	gameContext.entityMan->getComponent<VelocityComponent>(pounceComp.id).velocity = { 0.f, 0.f };

	// Reduce mucho la velocidad del objetivo
	float velocityReduction = 0.05f;
	gameContext.entityMan->getComponent<VelocityComponent>(pounceComp.objectiveId).velocity.x = gameContext.entityMan->getComponent<VelocityComponent>(pounceComp.objectiveId).velocity.x * velocityReduction;

	// Impedir que saltes
	if (gameContext.entityMan->existsComponent<JumpComponent>(pounceComp.objectiveId)) {
		JumpComponent& jumpComp = gameContext.entityMan->getComponent<JumpComponent>(pounceComp.objectiveId);

		jumpComp.cooldown = 0.f;
	}
}
