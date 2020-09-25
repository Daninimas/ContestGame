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

	for (int distanceWeapId : WorldData::worldDistanceWeapons)
	{
		ColliderComponent& weaponCollider = gameContext.entityMan.getComponent<ColliderComponent>(distanceWeapId);

		// Supongo que el arma solo tiene un collidable sin bounding hijos
		if (checkWeaponCollidesWithPlayer(weaponCollider.boundingRoot)) {
			setDistanceWeaponToPlayer(gameContext, distanceWeapId);

			break;
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
	gameContext.entityMan.getComponent<DistanceWeaponComponent>(distanceWeapId).id = WorldData::playerId;

	// Delete the weapon entity
	gameContext.eraseEntityByID(distanceWeapId);

	// Delete id from world data
	WorldData::worldDistanceWeapons.erase(std::remove(WorldData::worldDistanceWeapons.begin(), WorldData::worldDistanceWeapons.end(), distanceWeapId), WorldData::worldDistanceWeapons.end());
}
