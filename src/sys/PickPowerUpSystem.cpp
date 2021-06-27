#include "PickPowerUpSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>
#include <iostream>
#include <limits>

PickPowerUpSystem::PickPowerUpSystem() {}

PickPowerUpSystem::~PickPowerUpSystem() {}


void PickPowerUpSystem::update(GameEngine& gameContext) const {
	auto& powerUps = gameContext.entityMan->getComponents<PowerUpComponent>();
	std::vector<int> powerUpsToDelete;


	powerUpsToDelete.reserve(powerUps.size());

	// Distance weapons
	for (PowerUpComponent& powerUp : powerUps)
	{
		ColliderComponent& powerUpCollider = gameContext.entityMan->getComponent<ColliderComponent>(powerUp.id);

		if (powerUpCollider.collide) {
			int entityColliding = checkPowerUpCollides(gameContext, powerUpCollider.boundingRoot);

			// Supongo que el arma solo tiene un collidable sin bounding hijos
			if (entityColliding != std::numeric_limits<int>::max() ) {
				setPowerUpToEntity(gameContext, powerUp, entityColliding, powerUpsToDelete);

				// Play pick powerup sound
				Sound pickPowerSound;
				pickPowerSound.soundPath = "./Media/Sound/UI/powerUp.wav";
				gameContext.getSoundFacadeRef().loadSound(pickPowerSound.soundPath);
				gameContext.getSoundFacadeRef().playSound(pickPowerSound);
			}
		}
	}


	// Now delete the activated powerUps
	for (int id : powerUpsToDelete) {
		gameContext.eraseEntityByID(id);
	}
}


int PickPowerUpSystem::checkPowerUpCollides(GameEngine& gameContext, BoundingBoxNode& powerBounding) const {
	for (int entityColliding : powerBounding.bounding.entitiesColliding) {
		if (gameContext.entityMan->getEntity(entityColliding).getType() != EntityType::WALL ) {
			return entityColliding;
		}
	}

	return std::numeric_limits<int>::max();
}



void PickPowerUpSystem::setPowerUpToEntity(GameEngine& gameContext, PowerUpComponent& powerUp, int entityColliding, std::vector<int>& powerUpsToDelete) const {

	switch (powerUp.type)
	{
	case PowerUpComponent::Shield:
		setShieldToEntity(gameContext, powerUp, entityColliding);
		break;

	case PowerUpComponent::Fury:
		setFuryToEntity(gameContext, powerUp, entityColliding);
		break;

	case PowerUpComponent::ExtraLife:
		gameContext.entityMan->getComponent<HealthComponent>(WorldElementsData::playerId).extraLifes++;
		break;

	case PowerUpComponent::Drone:
		SituationComponent& powerSit = gameContext.entityMan->getComponent<SituationComponent>(powerUp.id);
		gameContext.entityMan->createDrone(gameContext, Vector2(powerSit.position.x, powerSit.position.y - 230.f), 0, GameObjectType::DRONE_FRIEND);
		break;
	}

	displayPowerUpNameText(gameContext, powerUp);

	powerUpsToDelete.emplace_back(powerUp.id);
}


void PickPowerUpSystem::setShieldToEntity(GameEngine& gameContext, PowerUpComponent& powerUp, int entityColliding) const {

	// Create shield entity and assing to entity
	GameObjectType shieldGO = GameObjectType::SHIELD;
	if (entityColliding == WorldElementsData::playerId) {
		shieldGO = GameObjectType::PLAYER_SHIELD;
	}

	int shieldId = gameContext.entityMan->createShield(gameContext, Vector2(10.f, 10.f), 0.f, shieldGO);
	ShieldComponent& shieldComp = gameContext.entityMan->getComponent<ShieldComponent>(shieldId);
	ColliderComponent& shieldColl = gameContext.entityMan->getComponent<ColliderComponent>(shieldId);
	ColliderComponent& objectiveColl = gameContext.entityMan->getComponent<ColliderComponent>(entityColliding);

	shieldComp.objectiveId = entityColliding;
	shieldColl.boundingRoot.bounding.xRight = (objectiveColl.boundingRoot.bounding.xRight - objectiveColl.boundingRoot.bounding.xLeft) * powerUp.shieldColliderIncFactor;
	shieldColl.boundingRoot.bounding.yDown  = (objectiveColl.boundingRoot.bounding.yDown  - objectiveColl.boundingRoot.bounding.yUp)   * powerUp.shieldColliderIncFactor;

	shieldComp.center = Utils::getCenterOfBounding(shieldColl.boundingRoot.bounding);
}


void PickPowerUpSystem::setFuryToEntity(GameEngine& gameContext, PowerUpComponent& powerUp, int entityColliding) const {
	//DELETE previous powerUp component on entity
	gameContext.entityMan->eraseComponent<FuryComponent>(entityColliding);

	// Set new fury component to entity
	FuryComponent& furyComp = gameContext.entityMan->createComponent<FuryComponent>(entityColliding);
	furyComp.timersSpeedIncFactor = powerUp.furyTimersSpeedIncFactor;
	furyComp.speedIncFactor = powerUp.furySpeedIncFactor;
	furyComp.totalLifeTime = powerUp.furyTotalLifeTime;
	furyComp.furyColor = powerUp.furyColor;
}


void PickPowerUpSystem::displayPowerUpNameText(GameEngine& gameContext, PowerUpComponent& powerUp) const {
	SituationComponent& powerUpSit = gameContext.entityMan->getComponent<SituationComponent>(powerUp.id);

	string text = powerUpTextMap.at(powerUp.type);

	gameContext.entityMan->createFloatingText(gameContext, powerUpSit.position, 0.f, text, { 255, 0, 0, 255 }, false, 20, 3.f, -40.f);
}