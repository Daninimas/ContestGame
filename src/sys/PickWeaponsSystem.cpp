#include "PickWeaponsSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>

#include <iostream>


PickWeaponsSystem::PickWeaponsSystem() {}

PickWeaponsSystem::~PickWeaponsSystem() {}

void PickWeaponsSystem::update(GameEngine& gameContext) const {
	
	checkPlayerPickingWeapons(gameContext);
}

void PickWeaponsSystem::checkPlayerPickingWeapons(GameEngine& gameContext) const {
	// Distance weapons
	for (int distanceWeapId : WorldData::worldDistanceWeapons)
	{
		ColliderComponent& weaponCollider = gameContext.entityMan.getComponent<ColliderComponent>(distanceWeapId);

		if (weaponCollider.collide) {
			// Supongo que el arma solo tiene un collidable sin bounding hijos
			if (checkWeaponCollidesWithPlayer(weaponCollider.boundingRoot)) {
				setDistanceWeaponToPlayer(gameContext, distanceWeapId);

				break;
			}
		}
	}

	// Melee weapons
	for (int meleeWeapId : WorldData::worldMeleeWeapons)
	{
		ColliderComponent& weaponCollider = gameContext.entityMan.getComponent<ColliderComponent>(meleeWeapId);

		if (weaponCollider.collide) {
			// Supongo que el arma solo tiene un collidable sin bounding hijos
			if (checkWeaponCollidesWithPlayer(weaponCollider.boundingRoot)) {
				setMeleeWeaponToPlayer(gameContext, meleeWeapId);

				break;
			}
		}
	}
}

bool PickWeaponsSystem::checkWeaponCollidesWithPlayer(BoundingBoxNode& weaponBounding) const {

	for (int entityColliding : weaponBounding.bounding.entitiesColliding) {
		if (entityColliding == WorldData::playerId) {
			return true;
		}
	}

	return false;
}



void PickWeaponsSystem::setDistanceWeaponToPlayer(GameEngine& gameContext, int distanceWeapId) const {
	//DELETE previous distance component
	gameContext.entityMan.eraseComponent<DistanceWeaponComponent>(WorldData::playerId);

	// Assign weapon component to the player
	DistanceWeaponComponent auxiliarCopy = gameContext.entityMan.getComponent<DistanceWeaponComponent>(distanceWeapId);

	DistanceWeaponComponent& playerDistance = gameContext.entityMan.createComponent<DistanceWeaponComponent>(WorldData::playerId);
	playerDistance = auxiliarCopy;
	playerDistance.id = WorldData::playerId;

	// Delete the weapon entity
	gameContext.eraseEntityByID(distanceWeapId);

	// Delete id from world data
	WorldData::worldDistanceWeapons.erase(std::remove(WorldData::worldDistanceWeapons.begin(), WorldData::worldDistanceWeapons.end(), distanceWeapId), WorldData::worldDistanceWeapons.end());
}

void PickWeaponsSystem::setMeleeWeaponToPlayer(GameEngine& gameContext, int meleeWeapId) const {

	//DELETE previous distance component
	gameContext.entityMan.eraseComponent<MeleeWeaponComponent>(WorldData::playerId);
	
	// Assign weapon component to the player
	MeleeWeaponComponent auxiliarCopy = gameContext.entityMan.getComponent<MeleeWeaponComponent>(meleeWeapId);

	MeleeWeaponComponent& playerMelee = gameContext.entityMan.createComponent<MeleeWeaponComponent>(WorldData::playerId);
	playerMelee = auxiliarCopy;
	playerMelee.id = WorldData::playerId;

	// Delete the weapon entity
	gameContext.eraseEntityByID(meleeWeapId);

	// Delete id from world data
	WorldData::worldMeleeWeapons.erase(std::remove(WorldData::worldMeleeWeapons.begin(), WorldData::worldMeleeWeapons.end(), meleeWeapId), WorldData::worldMeleeWeapons.end());
}