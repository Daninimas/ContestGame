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

	// ------------------------ COLLIDES ONLY WITH VELOCITY ENTITIES ----------------------------------------- 
/*void SensorSystem::checkSensorsCollisions(GameEngine& gameContext) const {
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

				calculateSensorCollision(gameContext, sensor, entityBounding, entitySituation);


			}
		}
	}
}*/

	// -------------------------- COLLIDES WITH ALL COLLIDERS ---------------------------------------------
void SensorSystem::checkSensorsCollisions(GameEngine& gameContext) const {
	auto& sensors   = gameContext.entityMan.getComponents<SensorComponent>();
	auto& colliders = gameContext.entityMan.getComponents<ColliderComponent>();

	for (SensorComponent& sensor : sensors) {
		// Check this sensor with all the colliders with VELOCITY
		BoundingBox sensorBounding = getSensorWorldFixedBounding(gameContext, sensor);

		for (ColliderComponent& colliderEnt : colliders) {

			if (colliderEnt.id != sensor.id && sensor.sensorLayerMasc & colliderEnt.collisionLayer) { // don't calculate the collision in the same entity
				SituationComponent& entitySituation = gameContext.entityMan.getComponent<SituationComponent>(colliderEnt.id);
				BoundingBox& entityBounding = colliderEnt.boundingRoot.bounding;

				calculateSensorCollision(gameContext, sensor, entityBounding, sensorBounding, entitySituation);
			}
		}
	}
}

BoundingBox SensorSystem::getSensorWorldFixedBounding(GameEngine& gameContext, SensorComponent& sensor) const {
	SituationComponent& sensorSit = gameContext.entityMan.getComponent<SituationComponent>(sensor.id);

	BoundingBox sensorWorldBounding = getSensorBoundingDependingFacing(gameContext, sensor, sensorSit);
	sensorWorldBounding = Utils::moveToWorldCoords(sensorWorldBounding, sensorSit);

	if (gameContext.entityMan.existsComponent<ColliderComponent>(sensor.id)) {
		Vector2 objectiveCenter = Utils::getCenterOfBounding(gameContext.entityMan.getComponent<ColliderComponent>(sensor.id).boundingRoot.bounding);
		sensorWorldBounding.xLeft + objectiveCenter.x;
		sensorWorldBounding.xRight + objectiveCenter.x;
	}


	return sensorWorldBounding;
}

bool SensorSystem::calculateSensorCollision(GameEngine& gameContext, SensorComponent& sensor, BoundingBox& sensorWorldBounding, BoundingBox& entityBounding, SituationComponent& entitySituation) const {
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


BoundingBox SensorSystem::getSensorBoundingDependingFacing(GameEngine& gameContext, SensorComponent& sensor, SituationComponent& sit) const {

	if (sit.facing == SituationComponent::Left) {
		BoundingBox auxBounding = sensor.sensorBounding;

		// The center of reflexion is the point of the entity
		auxBounding.xRight =  - sensor.sensorBounding.xLeft;
		auxBounding.xLeft =  - sensor.sensorBounding.xRight;

		return auxBounding;
	}
	
	return sensor.sensorBounding;
}