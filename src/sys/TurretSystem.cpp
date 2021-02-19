#include "TurretSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>

#include <iostream>


TurretSystem::TurretSystem() {}

TurretSystem::~TurretSystem() {}


void TurretSystem::update(GameEngine& gameContext) const {
	auto& turrets = gameContext.entityMan.getComponents<TurretComponent>();

	for (TurretComponent& turret : turrets) {
		manageTurret(gameContext, turret);
	}
}



void TurretSystem::manageTurret(GameEngine& gameContext, TurretComponent& turret) const {
	if (!turret.inUse) {
		checkIfPlayerUsesTurret(gameContext, turret);
	}
	else {
		manageTurretUsage(gameContext, turret);
	}
}


void TurretSystem::checkIfPlayerUsesTurret(GameEngine& gameContext, TurretComponent& turret) const {
	int playerId = WorldElementsData::playerId;
	InputComponent& playerInput = gameContext.entityMan.getComponent<InputComponent>(playerId);

	if (!playerInput.usingTurret) {
		SensorComponent& playerSensor = gameContext.entityMan.getComponent<SensorComponent>(playerId);

		if (Utils::isEntitySensoredBy(playerSensor, turret.id)) {
			// Hacer que salga un cartelito para entrar


			if (playerInput.attacking) {
				enterInTurret(gameContext, turret, playerId);

				playerInput.usingTurret = true;
			}
		}
	}
}


void TurretSystem::enterInTurret(GameEngine& gameContext, TurretComponent& turret, int userID) const {
	ColliderComponent& turretColl = gameContext.entityMan.getComponent<ColliderComponent>(turret.id);
	ColliderComponent& userColl   = gameContext.entityMan.getComponent<ColliderComponent>(userID);
	SituationComponent& userSit   = gameContext.entityMan.getComponent<SituationComponent>(userID);
	SituationComponent& turretSit = gameContext.entityMan.getComponent<SituationComponent>(turret.id);

	// Collider
	turretColl.layerMasc = userColl.layerMasc;
	userColl.weight = 10000000.f;  // Nothing hast to move the user

	// Turret
	turret.inUse = true;
	turret.userID = userID;

	// Situation
	if (turretSit.facing == SituationComponent::Left) {
		userSit.position.x = turretSit.position.x + turret.offsetX;
		userSit.facing = SituationComponent::Left;
	}
	else {
		userSit.position.x = turretSit.position.x + turretColl.boundingRoot.bounding.xRight - turret.offsetX - userColl.boundingRoot.bounding.xRight;
		userSit.facing = SituationComponent::Right;
	}


	std::cout << "entro en la torreta\n";
}


void TurretSystem::manageTurretUsage(GameEngine& gameContext, TurretComponent& turret) const {
	InputComponent& userInput = gameContext.entityMan.getComponent<InputComponent>(turret.userID);

	if (userInput.jumping) {
		exitTurret(gameContext, userInput, turret);

		return;
	}
}

void TurretSystem::exitTurret(GameEngine& gameContext, InputComponent& userInput, TurretComponent& turret) const {
	ColliderComponent& turretColl = gameContext.entityMan.getComponent<ColliderComponent>(turret.id);
	ColliderComponent& userColl = gameContext.entityMan.getComponent<ColliderComponent>(userInput.id);

	// Input 
	userInput.usingTurret = false;

	// userColl
	userColl.weight = 2.f;

	// turretColl
	turretColl.layerMasc = ColliderComponent::NoLayer;

	// Turret
	turret.inUse = false;

	std::cout << "salgo de la torreta\n";
}
