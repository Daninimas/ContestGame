#include "AIDroneSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>
#include <iostream>
#include <stdlib.h>
#include <math.h>

AIDroneSystem::AIDroneSystem() {}

AIDroneSystem::~AIDroneSystem() {}


void AIDroneSystem::update(GameEngine& gameContext) const {
	auto& flyingEntities = gameContext.entityMan->getComponents<AIFlyingChaseComponent>();

	for (AIFlyingChaseComponent& flyingEntity : flyingEntities) {
		if ( gameContext.entityMan->getEntity(flyingEntity.id).getType() == EntityType::DRONE ) {
			AIDistanceAtkComponent& distanceAIComp = gameContext.entityMan->getComponent<AIDistanceAtkComponent>(WorldElementsData::playerDroneId);

			if (!objectiveInsideSensor(gameContext, distanceAIComp)) {
				// The objective is dead or outside of the sensor
				setNewObjective(gameContext, distanceAIComp);
			}
		}
	}
}


bool AIDroneSystem::objectiveInsideSensor(GameEngine& gameContext, AIDistanceAtkComponent& distanceAIComp) const {
	SensorComponent& droneSensor = gameContext.entityMan->getComponent<SensorComponent>(distanceAIComp.id);

	for (int& sensoredEntId : droneSensor.entitiesSensoring) {
		if (sensoredEntId == distanceAIComp.objectiveId) {
			return true;
		}
	}

	return false;
}

void AIDroneSystem::setNewObjective(GameEngine& gameContext, AIDistanceAtkComponent& distanceAIComp) const {
	SensorComponent& droneSensor = gameContext.entityMan->getComponent<SensorComponent>(distanceAIComp.id);

	// Sets the first entity being sensed as objective
	if( droneSensor.entitiesSensoring.size() > 0 )
		distanceAIComp.objectiveId = droneSensor.entitiesSensoring.front();
}