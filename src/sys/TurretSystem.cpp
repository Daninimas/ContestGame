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

	// Collider
	turretColl.layerMasc = userColl.layerMasc;
	userColl.weight = 10000000.f;  // Nothing hast to move the user

	// Turret
	turret.inUse = true;
	turret.userID = userID;

}
