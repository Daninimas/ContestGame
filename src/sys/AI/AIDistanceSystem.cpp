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

	

	if (Utils::objectiveInsideRange(attackerSit, objectiveSit, AImeleeComp.range) && distWeap.cooldown > distWeap.maxCooldown) {
		AImeleeComp.createAttack = true;

		setAttackDirection(gameContext, attackerSit, objectiveSit, distWeap);
	}
}


void AIDistanceSystem::setAttackDirection(GameEngine& gameContext, SituationComponent& attackerSit, SituationComponent& objectiveSit, DistanceWeaponComponent& distWeap) const {
	Vector2 objectiveCenter = Utils::getCenterOfBounding(gameContext.entityMan.getComponent<ColliderComponent>(objectiveSit.id).boundingRoot.bounding);
	Vector2 dir;

	// Calculate direction vector
	dir.x = (objectiveSit.position.x + objectiveCenter.x) - attackerSit.position.x;
	dir.y = (objectiveSit.position.y + objectiveCenter.y) - attackerSit.position.y;

	// Normalize direction vector
	float mag = Utils::getVectorMagnitude(dir);
	
	if (mag > 0) {
		dir.x = dir.x / mag;
		dir.y = dir.y / mag;
	}

	// Set velocity to weapon
	distWeap.attackVel.x = distWeap.attackGeneralVelociy * dir.x;
	distWeap.attackVel.y = distWeap.attackGeneralVelociy * dir.y;
}