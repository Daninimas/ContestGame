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
	for (int distanceWeapId : WorldElementsData::worldDistanceWeapons)
	{
		ColliderComponent& weaponCollider = gameContext.entityMan->getComponent<ColliderComponent>(distanceWeapId);

		if (weaponCollider.collide) {
			// Supongo que el arma solo tiene un collidable sin bounding hijos
			if (Utils::checkCollidingWithEntity(weaponCollider.boundingRoot, WorldElementsData::playerId)/*checkWeaponCollidesWithPlayer(weaponCollider.boundingRoot)*/) {
				setDistanceWeaponToPlayer(gameContext, distanceWeapId);

				break;
			}
		}
	}

	// Melee weapons
	for (int meleeWeapId : WorldElementsData::worldMeleeWeapons)
	{
		ColliderComponent& weaponCollider = gameContext.entityMan->getComponent<ColliderComponent>(meleeWeapId);

		if (weaponCollider.collide) {
			// Supongo que el arma solo tiene un collidable sin bounding hijos
			if (Utils::checkCollidingWithEntity(weaponCollider.boundingRoot, WorldElementsData::playerId)/*checkWeaponCollidesWithPlayer(weaponCollider.boundingRoot)*/) {
				setMeleeWeaponToPlayer(gameContext, meleeWeapId);

				break;
			}
		}
	}
}
/*
bool PickWeaponsSystem::checkWeaponCollidesWithPlayer(BoundingBoxNode& weaponBounding) const {

	for (int entityColliding : weaponBounding.bounding.entitiesColliding) {
		if (entityColliding == WorldElementsData::playerId) {
			return true;
		}
	}

	return false;
}
*/


void PickWeaponsSystem::setDistanceWeaponToPlayer(GameEngine& gameContext, int distanceWeapId) const {
	displayWeaponNameText(gameContext, distanceWeapId);
	//DELETE previous distance component
	gameContext.entityMan->eraseComponent<DistanceWeaponComponent>(WorldElementsData::playerId);

	// Assign weapon component to the player
	DistanceWeaponComponent auxiliarCopy = gameContext.entityMan->getComponent<DistanceWeaponComponent>(distanceWeapId);

	DistanceWeaponComponent& playerDistance = gameContext.entityMan->createComponent<DistanceWeaponComponent>(WorldElementsData::playerId);
	playerDistance = auxiliarCopy;
	playerDistance.id = WorldElementsData::playerId;

	// Delete the weapon entity
	gameContext.eraseEntityByID(distanceWeapId);

	// Delete id from world data
	WorldElementsData::worldDistanceWeapons.erase(std::remove(WorldElementsData::worldDistanceWeapons.begin(), WorldElementsData::worldDistanceWeapons.end(), distanceWeapId), WorldElementsData::worldDistanceWeapons.end());
}

void PickWeaponsSystem::setMeleeWeaponToPlayer(GameEngine& gameContext, int meleeWeapId) const {
	displayWeaponNameText(gameContext, meleeWeapId);
	//DELETE previous distance component
	gameContext.entityMan->eraseComponent<MeleeWeaponComponent>(WorldElementsData::playerId);
	
	// Assign weapon component to the player
	MeleeWeaponComponent auxiliarCopy = gameContext.entityMan->getComponent<MeleeWeaponComponent>(meleeWeapId);

	MeleeWeaponComponent& playerMelee = gameContext.entityMan->createComponent<MeleeWeaponComponent>(WorldElementsData::playerId);
	playerMelee = auxiliarCopy;
	playerMelee.id = WorldElementsData::playerId;

	// Delete the weapon entity
	gameContext.eraseEntityByID(meleeWeapId);

	// Delete id from world data
	WorldElementsData::worldMeleeWeapons.erase(std::remove(WorldElementsData::worldMeleeWeapons.begin(), WorldElementsData::worldMeleeWeapons.end(), meleeWeapId), WorldElementsData::worldMeleeWeapons.end());
}

void PickWeaponsSystem::displayWeaponNameText(GameEngine& gameContext, int weaponId) const {
	SituationComponent& weaponSit = gameContext.entityMan->getComponent<SituationComponent>(weaponId);

	string text = weaponTextMap.at(gameContext.entityMan->getEntity(weaponId).getGameObjectType());

	gameContext.entityMan->createFloatingText(gameContext, weaponSit.position, 0.f, text, {255, 0, 0, 255}, false, 20, 2.5f, -40.f);
}