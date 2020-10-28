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
	// Set the plyer in its center
	SituationComponent& playerSit = gameContext.entityMan.getComponent<SituationComponent>(WorldData::playerId);
	SituationComponent& cameraSit = gameContext.entityMan.getComponent<SituationComponent>(WorldData::activeCameraId);
	CameraComponent&   cameraComp = gameContext.entityMan.getComponent<CameraComponent>(WorldData::activeCameraId);

	auto moveCamera = [](float posPlayer, float& posCamera, float offset) {
		// Calculate distance from player to camera
		float dis = posPlayer - posCamera;
		// Check if distance is inside offset
		float diff = abs(dis) - offset;
		if (diff > 0.f) {
			// Add to the camera that possition

			posCamera += (Utils::sign(dis) * diff);
		}
	};


	moveCamera(playerSit.x, cameraSit.x, cameraComp.offsetX);
	moveCamera(playerSit.y, cameraSit.y, cameraComp.offsetY);

	// Update position on engine, the situation of the camera is its center
	gameContext.getWindowFacadeRef().updateCamera(gameContext, WorldData::activeCameraId);
}