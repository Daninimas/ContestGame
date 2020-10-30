#include "AIDistanceSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>
#include <iostream>
#include <stdlib.h>
#include <math.h>

AIDistanceSystem::AIDistanceSystem() {}

AIDistanceSystem::~AIDistanceSystem() {}


void AIDistanceSystem::update(GameEngine& gameContext) const {

	auto& AIDistanceAtkComponents = gameContext.entityMan.getComponents<AIDistanceAtkComponent>();

	for (AIDistanceAtkComponent& AIdistanceComp : AIDistanceAtkComponents)
	{
		AIdistanceComp.createAttack = false;

		if (gameContext.entityMan.existsComponent<DistanceWeaponComponent>(AIdistanceComp.id) && gameContext.entityMan.existsComponent<SituationComponent>(AIdistanceComp.objectiveId)) {
			manageAttackGeneration(gameContext, AIdistanceComp);
		}
	}
}


void AIDistanceSystem::manageAttackGeneration(GameEngine& gameContext, AIDistanceAtkComponent& AImeleeComp) const {
	SituationComponent&   attackerSit  = gameContext.entityMan.getComponent<SituationComponent>(AImeleeComp.id);
	SituationComponent&   objectiveSit = gameContext.entityMan.getComponent<SituationComponent>(AImeleeComp.objectiveId);
	DistanceWeaponComponent& distWeap  = gameContext.entityMan.getComponent<DistanceWeaponComponent>(AImeleeComp.id);

	

	if (Utils::objectiveInsideRange(attackerSit, objectiveSit, AImeleeComp.rangeX, AImeleeComp.rangeY) && distWeap.cooldown > distWeap.maxCooldown) {
		AImeleeComp.createAttack = true;

		setAttackDirection(gameContext, attackerSit, objectiveSit, distWeap);
	}
}


void AIDistanceSystem::setAttackDirection(GameEngine& gameContext, SituationComponent& attackerSit, SituationComponent& objectiveSit, DistanceWeaponComponent& distWeap) const {
	float dirX, dirY = 0.f;

	// Calculate direction vector
	dirX = objectiveSit.x - attackerSit.x;
	dirY = objectiveSit.y - attackerSit.y;

	// Normalize direction vector
	float mag = sqrt(dirX * dirX + dirY * dirY); // Calculate magnitude
	if (mag > 0) { 
		dirX = dirX / mag;
		dirY = dirY / mag;
	}

	// Set velocity to weapon
	distWeap.attackVelX = distWeap.attackGeneralVelociy * dirX;
	distWeap.attackVelY = distWeap.attackGeneralVelociy * dirY;
}