#include "CameraSystem.hpp"

#include <eng/GameEngine.hpp>
#include <iostream>
#include <math.h>
#include <tools/Utils.hpp>

CameraSystem::CameraSystem() {}

CameraSystem::~CameraSystem() {}


void CameraSystem::update(GameEngine& gameContext) const {
	setCameraLookingPlayer(gameContext);
}

void CameraSystem::setCameraLookingPlayer(GameEngine& gameContext) const {
	// Set the player in its center
	SituationComponent& playerSit = gameContext.entityMan.getComponent<SituationComponent>(WorldElementsData::playerId);
	SituationComponent& cameraSit = gameContext.entityMan.getComponent<SituationComponent>(WorldElementsData::activeCameraId);
	CameraComponent&   cameraComp = gameContext.entityMan.getComponent<CameraComponent>(WorldElementsData::activeCameraId);

	auto moveCamera = [](float posPlayer, float& posCamera, float offset) {
		// Calculate distance from player to camera
		float dis = posPlayer - posCamera;
		// Check if distance is inside offset
		float diff = abs(dis) - offset;
		if (diff > 0.f) {
			// Add to the camera that position

			posCamera += (Utils::sign(dis) * diff);
		}
	};


	moveCamera(playerSit.position.x, cameraSit.position.x, cameraComp.offset.x);
	//moveCamera(playerSit.position.y, cameraSit.position.y, cameraComp.offset.y);
	cameraSit.position.y = playerSit.position.y;
	
	// Update position on engine, the situation of the camera is its center
	gameContext.entityMan.addEntityToUpdate(WorldElementsData::activeCameraId);
}