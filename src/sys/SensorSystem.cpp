#include "SensorSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>

#include <iostream>


SensorSystem::SensorSystem() {}

SensorSystem::~SensorSystem() {}


void SensorSystem::update(GameEngine& gameContext) const {
	// Reset previous entities sensored
	resetSensors(gameContext);

	checkSensorsCollisions(gameContext);
}

void SensorSystem::resetSensors(GameEngine& gameContext) const {
	auto& sensors = gameContext.entityMan.getComponents<SensorComponent>();

	for (SensorComponent& sensor : sensors) {
		sensor.entitiesSensoring.clear();
	}
}

void SensorSystem::checkSensorsCollisions(GameEngine& gameContext) const {
	auto& sensors = gameContext.entityMan.getComponents<SensorComponent>();
	std::vector<int> idCollidersWithVelocity;


	idCollidersWithVelocity.reserve( gameContext.entityMan.getComponents<VelocityComponent>().size() ); // Reserve the size of the velocities component storage
	Utils::insertCollidersIdWithVelocity(gameContext, idCollidersWithVelocity);

	for (SensorComponent& sensor : sensors) {
		// Check this sensor with all the colliders with VELOCITY

		for (int velEntityID : idCollidersWithVelocity) {

			if (velEntityID != sensor.id) { // don't calculate the collision in the same entity
				SituationComponent& entitySituation = gameContext.entityMan.getComponent<SituationComponent>(velEntityID);
				BoundingBox& entityBounding = gameContext.entityMan.getComponent<ColliderComponent>(velEntityID).boundingRoot.bounding;

				calculateSensorCollision(gameContext, sensor, entityBounding, entitySituation);
			}
		}
	}
}

bool SensorSystem::calculateSensorCollision(GameEngine& gameContext, SensorComponent& sensor, BoundingBox& entityBounding, SituationComponent& entitySituation) const {
	return false;
}

