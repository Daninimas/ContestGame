#include "OrbitalWeaponSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>
#include <iostream>

OrbitalWeaponSystem::OrbitalWeaponSystem() {}

OrbitalWeaponSystem::~OrbitalWeaponSystem() {}


void OrbitalWeaponSystem::update(GameEngine& gameContext) const {
	addCooldownToOrbitalWeapons(gameContext);

	checkEnemiesAttacking(gameContext);

	manageOrbitalStrikes(gameContext);
}

void OrbitalWeaponSystem::addCooldownToOrbitalWeapons(GameEngine& gameContext) const {
	float deltaTime = gameContext.getDeltaTime();
	auto& orbitalWeapons = gameContext.entityMan.getComponents<OrbitalWeaponComponent>();

	for (auto& orbitalWeap : orbitalWeapons) {
		orbitalWeap.cooldown += deltaTime;
	}
}


void OrbitalWeaponSystem::checkEnemiesAttacking(GameEngine& gameContext) const {
	
}

void OrbitalWeaponSystem::generateOrbitalMarker(GameEngine& gameContext, OrbitalWeaponComponent& orbitalWeapon, SituationComponent& objectiveSituation) const {
	int markerID = gameContext.entityMan.createOrbitalMarker(gameContext, objectiveSituation.position, GameObjectType::ORBITAL_MARKER);

	AutodeleteComponent& autodeleteComp = gameContext.entityMan.getComponent<AutodeleteComponent>(markerID);
	RenderComponent& renderComp = gameContext.entityMan.getComponent<RenderComponent>(markerID);

	calculateBoundingToFloor(gameContext, objectiveSituation, orbitalWeapon);

	autodeleteComp.timeToDelete = orbitalWeapon.generateAttackTime;

	renderComp.spriteRect = orbitalWeapon.attackBounding; // The attack bounding previous calculated with the distance to the floor
	renderComp.spriteRect.xLeft = 0.f;
	renderComp.spriteRect.xRight = 30.f;

	// Set the counters for the attack
	orbitalWeapon.activated = true;
	orbitalWeapon.cooldown = 0.f;
	orbitalWeapon.generateAttackTimeCounter = 0.f;
	orbitalWeapon.attackPosition = objectiveSituation.position;

	// Update render
	gameContext.entityMan.addEntityToUpdate(markerID);

	// Play orbital marker sound
	gameContext.getSoundFacadeRef().loadSound(orbitalWeapon.markerSound.soundPath);
	gameContext.getSoundFacadeRef().playSound(orbitalWeapon.markerSound);
}




void OrbitalWeaponSystem::manageOrbitalStrikes(GameEngine& gameContext) const {
	auto& orbitalWeapons = gameContext.entityMan.getComponents<OrbitalWeaponComponent>();
	float deltaTime = gameContext.getDeltaTime();

	for (OrbitalWeaponComponent& orbitalWeapon : orbitalWeapons) {
		if (orbitalWeapon.activated) {
			orbitalWeapon.generateAttackTimeCounter += deltaTime;

			if (orbitalWeapon.generateAttackTimeCounter > orbitalWeapon.generateAttackTime) {
				generateOrbitalAttack(gameContext, orbitalWeapon);
			}
		}
	}
}



void OrbitalWeaponSystem::generateOrbitalAttack(GameEngine& gameContext, OrbitalWeaponComponent& orbitalWeapon) const {
	GameObjectType attackGOtype = GameObjectType::MELEE_ATTACK;
	if (orbitalWeapon.id == WorldElementsData::playerId) {
		attackGOtype = GameObjectType::PLAYER_MELEE_ATTACK;
	}

	int attackId = gameContext.entityMan.createAttack(gameContext, orbitalWeapon.attackPosition, 0.f, attackGOtype);

	ColliderComponent& colliderComp = gameContext.entityMan.getComponent<ColliderComponent>(attackId);
	AttackComponent& attackComp = gameContext.entityMan.getComponent<AttackComponent>(attackId);

	colliderComp.boundingRoot.bounding = orbitalWeapon.attackBounding;
	attackComp.damage = orbitalWeapon.damage;
	attackComp.maxLifetime = orbitalWeapon.attackLifetime;


	orbitalWeapon.generateAttackTimeCounter = 0.f;
	orbitalWeapon.activated = false;

	// Play orbital attack sound
	gameContext.getSoundFacadeRef().loadSound(orbitalWeapon.attackSound.soundPath);
	gameContext.getSoundFacadeRef().playSound(orbitalWeapon.attackSound);
}



void OrbitalWeaponSystem::calculateBoundingToFloor(GameEngine& gameContext, SituationComponent& objectiveSituation, OrbitalWeaponComponent& orbitalWeapon) const { // changes the orbital Weapon attack bounding to the floor from the objective
	SituationComponent* closestWall = Utils::getClosestWallXToObjetive(gameContext, objectiveSituation);

	orbitalWeapon.attackBounding.yUp = -100000.f;

	if (closestWall != nullptr) {
		// Check if the situation of the objective is inside of the size x of the wall
		ColliderComponent& wallCollider = gameContext.entityMan.getComponent<ColliderComponent>(closestWall->id);

		BoundingBox wallWorldBounding = Utils::moveToWorldCoords(wallCollider.boundingRoot.bounding, *closestWall);

		if (objectiveSituation.position.x >= wallWorldBounding.xLeft && objectiveSituation.position.x <= wallWorldBounding.xRight) {
			// Data when there is floor
			orbitalWeapon.attackBounding.yDown = closestWall->position.y - objectiveSituation.position.y;

		}
		else {
			// Data when there is no floor
			orbitalWeapon.attackBounding.yDown = 100000.f;
		}
	}
	else {
		// Data when there is no floor
		orbitalWeapon.attackBounding.yDown =  100000.f;
	}
}
