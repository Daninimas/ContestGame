#include "WorldSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>

#include <iostream>


WorldSystem::WorldSystem() {}

WorldSystem::~WorldSystem() {}


void WorldSystem::update(GameEngine& gameContext) const {
	// Reset previous entities sensed
	deleteEntitiesOutOfWorld(gameContext);

	checkPhaseCollision(gameContext);
}

void WorldSystem::deleteEntitiesOutOfWorld(GameEngine& gameContext) const {
	auto& situations = gameContext.entityMan.getComponents<SituationComponent>();
	std::vector<int> entitiesToDelete{};
	BoundingBox& worldBounding = WorldData::phaseLimits;

	entitiesToDelete.reserve(situations.size());
	for (SituationComponent& situation : situations) {
		if (situation.position.x < worldBounding.xLeft || situation.position.x > worldBounding.xRight || situation.position.y < worldBounding.yUp || situation.position.y > worldBounding.yDown) {

			if (!situation.noWorldDelete) {
				entitiesToDelete.push_back(situation.id);
			}
			else {
				// TODO do something with the player when its outside of world
			}

		}
	}

	// Delete the entities outside
	for (size_t i = 0; i < entitiesToDelete.size(); ++i)
	{
		gameContext.eraseEntityByID(entitiesToDelete[i]);
	}
}




void WorldSystem::checkPhaseCollision(GameEngine& gameContext) const {
	// Calculate player collision with phase
	collideWithPhaseBounding(gameContext, WorldData::playerId, gameContext.entityMan.getComponent<ColliderComponent>(WorldData::playerId).boundingRoot.bounding);

	// Calculate camera collision with phase
	CameraComponent& cameraComp = gameContext.entityMan.getComponent<CameraComponent>(WorldData::activeCameraId);
	BoundingBox cameraBounding = Utils::getCameraViewBoundig(cameraComp);

	collideWithPhaseBounding(gameContext, WorldData::activeCameraId, cameraBounding);
}


void WorldSystem::collideWithPhaseBounding(GameEngine& gameContext, int entityId, BoundingBox& entityBounding) const {
	// make the player and camera don't get out of the phase zone
	BoundingBox& worldBounding = WorldData::phaseLimits;
	SituationComponent& entitySit = gameContext.entityMan.getComponent<SituationComponent>(entityId);
	BoundingBox entityWorldBox = Utils::moveToWorldCoords(entityBounding, entitySit);

	auto calculateIntersection = [](float mobLeft, float mobRight, float worldLeft, float worldRight) -> float { // trailing return type
		if (mobLeft < worldLeft) {
			return worldLeft - mobLeft;
		}
		else if (mobRight > worldRight) {
			return worldRight - mobRight;
		}

		return 0.f;
	};

	float overlapX = calculateIntersection(entityWorldBox.xLeft, entityWorldBox.xRight, worldBounding.xLeft, worldBounding.xRight);
	float overlapY = calculateIntersection(entityWorldBox.yUp, entityWorldBox.yDown, worldBounding.yUp, worldBounding.yDown);

	if (overlapX != 0.f || overlapY != 0.f) {
		// Resolve overlap situation
		entitySit.position.x += overlapX;
		entitySit.position.y += overlapY;

		// Make velocity 0 when intersect
		if (gameContext.entityMan.existsComponent<VelocityComponent>(entityId)) {
			VelocityComponent& entityVelocity = gameContext.entityMan.getComponent<VelocityComponent>(entityId);

			if (entityVelocity.velocity.x < 0.f) {  // moving left
				if (overlapX > 0.f) {
					entityVelocity.velocity.x = 0.f;
				}
			}
			else {
				if (overlapX < 0.f) {               // moving right
					entityVelocity.velocity.x = 0.f;
				}
			}


			if (entityVelocity.velocity.y < 0.f) {  // moving up
				if (overlapY > 0.f) {
					entityVelocity.velocity.y = 0.f;
				}
			}
			else {
				if (overlapY < 0.f) {              // moving down
					entityVelocity.velocity.y = 0.f;
				}
			}
		}

		gameContext.entityMan.addEntityToUpdate(entityId);
	}
}
