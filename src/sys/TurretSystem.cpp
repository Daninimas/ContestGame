#include "TurretSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>

#include <iostream>


TurretSystem::TurretSystem() {}

TurretSystem::~TurretSystem() {}


void TurretSystem::update(GameEngine& gameContext) const {
	auto& turrets = gameContext.entityMan.getComponents<TurretComponent>();

	for (TurretComponent& turret : turrets) {
		if (!turret.disabled) {
			manageTurret(gameContext, turret);
		}
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
	DistanceWeaponComponent& turretWeapon = gameContext.entityMan.getComponent<DistanceWeaponComponent>(turret.turretGunID);

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

	// Distasnce weapon
	// For solving the problem of shooting when the player enters
	turretWeapon.cooldown = 0.f;


	std::cout << "entro en la torreta\n";
}


void TurretSystem::manageTurretUsage(GameEngine& gameContext, TurretComponent& turret) const {
	InputComponent& userInput = gameContext.entityMan.getComponent<InputComponent>(turret.userID);

	if (userInput.jumping) {
		exitTurret(gameContext, userInput, turret);

		return;
	}

	manageGunRotation(gameContext, turret, userInput);

	if (userInput.attacking) {
		manageShoot(gameContext, turret);
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


void TurretSystem::manageShoot(GameEngine& gameContext, TurretComponent& turret) const {
	DistanceWeaponComponent& turretWeapon = gameContext.entityMan.getComponent<DistanceWeaponComponent>(turret.turretGunID);
	GunTurretComponent& gunTurretComp = gameContext.entityMan.getComponent<GunTurretComponent>(turret.turretGunID);

	if (turretWeapon.cooldown > turretWeapon.maxCooldown) {
		// Set velocity to weapon
		turretWeapon.attackVel.x = turretWeapon.attackGeneralVelociy * sin(Utils::degToRad(gunTurretComp.currentRotation + 90.f));
		turretWeapon.attackVel.y = turretWeapon.attackGeneralVelociy * cos(Utils::degToRad(gunTurretComp.currentRotation + 90.f));

		gunTurretComp.createAttack = true;
	}
}


void TurretSystem::manageGunRotation(GameEngine& gameContext, TurretComponent& turret, InputComponent& userInput) const {
	GunTurretComponent& gunTurretComp = gameContext.entityMan.getComponent<GunTurretComponent>(turret.turretGunID);
	SituationComponent& TurretGunSit = gameContext.entityMan.getComponent<SituationComponent>(turret.turretGunID);
	float inverted = 1.f; // Es o 1 o -1, para que dependiendo el facing el subir o el bajar hagan lo contrario

	if (TurretGunSit.facing == SituationComponent::Left) {
		inverted = -1.f;
	}

	if (userInput.movingDown) {
		gunTurretComp.currentRotation -= gunTurretComp.gunRotationSpeed * gameContext.getDeltaTime() * inverted;
	}
	if (userInput.movingUp) {
		gunTurretComp.currentRotation += gunTurretComp.gunRotationSpeed * gameContext.getDeltaTime() * inverted;
	}

	gunTurretComp.currentRotation = std::clamp(gunTurretComp.currentRotation, gunTurretComp.minRotation, gunTurretComp.maxRotation);
	TurretGunSit.rotation = gunTurretComp.currentRotation;

	gameContext.entityMan.addEntityToUpdate(turret.turretGunID); // Update canon in the sfml engine
}
