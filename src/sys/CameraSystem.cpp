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
	VelocityComponent& cameraVel  = gameContext.entityMan.getComponent<VelocityComponent>(WorldElementsData::activeCameraId);
	Vector2 cameraObjectivePos = cameraSit.position;

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
	
	// Get the camera objective position
	moveCamera(playerSit.position.x + cameraComp.cameraAdvancement, cameraObjectivePos.x, cameraComp.offset.x);
	//moveCamera(playerSit.position.y, cameraSit.position.y, cameraComp.offset.y);
	cameraSit.position.y = playerSit.position.y;

	// Set the velocity to the camera
	float distance = cameraObjectivePos.x - cameraSit.position.x;
	if (abs(distance) < 10.f) {
		distance = 0.f;
	}

	cameraVel.velocity.x = std::clamp(distance * 3.f, -cameraVel.speedX, cameraVel.speedX);
	//cameraVel.velocity.y = cameraVel.speedX * (int)(cameraSit.position.y - cameraObjectivePos.y);
	
	/*std::cout << "----------------------------------------------\n";
	std::cout << "Camera position: ( " << cameraSit.position.x << ", " << cameraSit.position.y << ")\n";
	std::cout << "Objective position: ( " << cameraObjectivePos.x << ", " << cameraObjectivePos.y << ")\n";
	std::cout << "Final camera Velocity: ( " << cameraVel.velocity.x << ", " << cameraVel.velocity.y << ")\n";*/

	// Update position on engine, the situation of the camera is its center
	gameContext.entityMan.addEntityToUpdate(WorldElementsData::activeCameraId);
}