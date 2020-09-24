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
			ColliderComponent& entityCollider = gameContext.entityMan.getComponent<ColliderComponent>(velEntityID);

			if (velEntityID != sensor.id && sensor.sensorLayerMasc & entityCollider.collisionLayer) { // don't calculate the collision in the same entity
				SituationComponent& entitySituation = gameContext.entityMan.getComponent<SituationComponent>(velEntityID);
				BoundingBox& entityBounding = entityCollider.boundingRoot.bounding;

				if (calculateSensorCollision(gameContext, sensor, entityBounding, entitySituation)) {
					//cout << "Sensor: " << sensor.id << " is colliding with: " << velEntityID;
				}

			}
		}
	}
}

bool SensorSystem::calculateSensorCollision(GameEngine& gameContext, SensorComponent& sensor, BoundingBox& entityBounding, SituationComponent& entitySituation) const {
	SituationComponent& sensorSit = gameContext.entityMan.getComponent<SituationComponent>(sensor.id);
	BoundingBox sensorBounding    = getSensorBoundingDependingFacing(gameContext, sensor);

	BoundingBox sensorWorldBounding = Utils::moveToWorldCoords(sensorBounding, sensorSit);
	BoundingBox entWorldBounding    = Utils::moveToWorldCoords(entityBounding, entitySituation);

	auto checkIntervals = [](float L1, float R1, float L2, float R2) {
		if (L2 >= R1) return false;
		if (L1 >= R2) return false;
		return true;
	};

	if (checkIntervals(sensorWorldBounding.xLeft, sensorWorldBounding.xRight, entWorldBounding.xLeft, entWorldBounding.xRight)
		&& checkIntervals(sensorWorldBounding.yUp, sensorWorldBounding.yDown, entWorldBounding.yUp, entWorldBounding.yDown)) {

		// The entity is inside the sensor
		sensor.entitiesSensoring.push_back(entitySituation.id);

		return true;
	}

	return false;
}


BoundingBox SensorSystem::getSensorBoundingDependingFacing(GameEngine& gameContext, SensorComponent& sensor) const {
	SituationComponent& sit = gameContext.entityMan.getComponent<SituationComponent>(sensor.id);

	if (sit.facing == SituationComponent::Left) {
		BoundingBox auxBounding = sensor.sensorBounding;

		auxBounding.xRight = sensor.sensorBounding.xLeft;
		auxBounding.xLeft = sensor.sensorBounding.xLeft - ( sensor.sensorBounding.xRight - sensor.sensorBounding.xLeft);

		return auxBounding;
	}
	
	return sensor.sensorBounding;
}