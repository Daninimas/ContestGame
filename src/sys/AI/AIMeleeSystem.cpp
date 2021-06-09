#include "AIMeleeSystem.hpp"

#include <eng/GameEngine.hpp>
#include <iostream>

AIMeleeSystem::AIMeleeSystem() {}

AIMeleeSystem::~AIMeleeSystem() {}


void AIMeleeSystem::update(GameEngine& gameContext) const {

	auto& AIMeleeAtkComponents = gameContext.entityMan->getComponents<AIMeleeAtkComponent>();

	for (AIMeleeAtkComponent& AImeleeComp : AIMeleeAtkComponents)
	{
		AImeleeComp.createAttack = false;

		if(gameContext.entityMan->existsComponent<SensorComponent>(AImeleeComp.id) && gameContext.entityMan->existsComponent<MeleeWeaponComponent>(AImeleeComp.id))
			manageAttacksGeneration(gameContext, AImeleeComp);
	}
}


void AIMeleeSystem::manageAttacksGeneration(GameEngine& gameContext, AIMeleeAtkComponent& AImeleeComp) const {
	SensorComponent& attackerSensor = gameContext.entityMan->getComponent<SensorComponent>(AImeleeComp.id);

	for (int& sensoredEntId : attackerSensor.entitiesSensoring) {
		if (sensoredEntId == AImeleeComp.objectiveId) {
			AImeleeComp.createAttack = true;
		}
	}
}
