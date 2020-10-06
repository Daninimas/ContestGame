#include "CameraSystem.hpp"

#include <eng/GameEngine.hpp>
#include <iostream>

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

	cameraSit.x = playerSit.x;
	cameraSit.y = playerSit.y;

	// Update position on engine
	gameContext.getWindowFacadeRef().updateCamera(gameContext, WorldData::activeCameraId);
}