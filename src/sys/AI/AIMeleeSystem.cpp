#include "AIMeleeSystem.hpp"

#include <eng/GameEngine.hpp>
#include <iostream>

AIMeleeSystem::AIMeleeSystem() {}

AIMeleeSystem::~AIMeleeSystem() {}


void AIMeleeSystem::update(GameEngine& gameContext) const {

	auto& AImeleeComponents = gameContext.entityMan.getComponents<AIMeleeComponent>();

	for (AIMeleeComponent& AImeleeComp : AImeleeComponents)
	{
		AImeleeComp.createAttack = false;

		if(gameContext.entityMan.existsComponent<SensorComponent>(AImeleeComp.id) && gameContext.entityMan.existsComponent<MeleeWeaponComponent>(AImeleeComp.id))
			manageAttacksGeneration(gameContext, AImeleeComp);
	}
}


void AIMeleeSystem::manageAttacksGeneration(GameEngine& gameContext, AIMeleeComponent& AImeleeComp) const {
	SensorComponent& attackerSensor = gameContext.entityMan.getComponent<SensorComponent>(AImeleeComp.id);

	for (int& sensoredEntId : attackerSensor.entitiesSensoring) {
		if (sensoredEntId == AImeleeComp.objectiveId) {
			AImeleeComp.createAttack = true;
		}
	}
}
