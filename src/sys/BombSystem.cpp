#include "BombSystem.hpp"

#include <eng/GameEngine.hpp>
#include <iostream>
#include <tools/Utils.hpp>

BombSystem::BombSystem() {}

BombSystem::~BombSystem() {}

void BombSystem::update(GameEngine& gameContext) const {
	auto& BombComponents = gameContext.entityMan->getComponents<BombComponent>();
	std::vector<int> bombsToExplode;
	bombsToExplode.reserve(BombComponents.size());

	for (BombComponent& bombComp : BombComponents) {
		// Update timer if is activated
		if (bombComp.activated) {
			updateBombTimers(gameContext, bombComp, bombsToExplode);
		}
		else {
			// if not activated, check collision to activate
			checkBombCollision(gameContext, bombComp);
		}

		if(gameContext.entityMan->existsComponent<VelocityComponent>(bombComp.id))
			setRotation(gameContext, bombComp);
	}

	for (int bombId : bombsToExplode) {
		explodeBomb(gameContext, bombId);
	}
}


void BombSystem::updateBombTimers(GameEngine& gameContext, BombComponent& bombComp, std::vector<int>& bombsToExplode) const {
	bombComp.currentTime += gameContext.getDeltaTime();

	if (bombComp.currentTime > bombComp.explosionTime) {
		bombsToExplode.push_back(bombComp.id);
	}
}

void BombSystem::checkBombCollision(GameEngine& gameContext, BombComponent& bombComp) const {
	ColliderComponent& collider = gameContext.entityMan->getComponent<ColliderComponent>(bombComp.id);

	if (collider.collide) {
		VelocityComponent& bombVel = gameContext.entityMan->getComponent<VelocityComponent>(bombComp.id);
		bombVel.velocity.x = 0;
	}

	bombComp.activated = collider.collide;
}

void BombSystem::explodeBomb(GameEngine& gameContext, int bombId) const {
	SituationComponent& bombSit   = gameContext.entityMan->getComponent<SituationComponent>(bombId);
	BombComponent& bombComp       = gameContext.entityMan->getComponent<BombComponent>(bombId);
	ColliderComponent& bombColl   = gameContext.entityMan->getComponent<ColliderComponent>(bombId);
	GameObjectType explosionGO    = GameObjectType::EXPLOSION;

	if (gameContext.entityMan->getEntity(bombId).getGameObjectType() == GameObjectType::PLAYER_BOMB) {
		GameObjectType explosionGO = GameObjectType::PLAYER_EXPLOSION;
	}
	// Creates the explosion attack entity
	int explosionId = gameContext.entityMan->createAttack(gameContext, bombSit.position, bombSit.rotation, explosionGO);

	ExplosionAttackComponent& explosionComp = gameContext.entityMan->getComponent<ExplosionAttackComponent>(explosionId);
	ColliderComponent& explosionCollider    = gameContext.entityMan->getComponent<ColliderComponent>(explosionId);
	AttackComponent& explosionAttack        = gameContext.entityMan->getComponent<AttackComponent>(explosionId);

	// ExplosionAttackComponent
	explosionComp.expansionVelocity = bombComp.explosionExpansion;

	// ColliderComponent
	BoundingBox& bombBounding = bombColl.boundingRoot.bounding;
	explosionCollider.boundingRoot.bounding = {bombBounding.xLeft, bombBounding.xRight, bombBounding.xLeft, bombBounding.xRight}; // To make it square
	explosionCollider.collisionLayer	    = bombColl.collisionLayer;
	explosionCollider.layerMasc             = bombColl.layerMasc;

	// AttackComponent
	explosionAttack.damage		= bombComp.damageExplosion;
	explosionAttack.maxLifetime = bombComp.explosionLifetime;

	// Play explosion sound
	gameContext.getSoundFacadeRef().loadSound(bombComp.explosionSound.soundPath);
	gameContext.getSoundFacadeRef().playSound(bombComp.explosionSound);

	// DELETE BOMB
	gameContext.eraseEntityByID(bombId);
}


void BombSystem::setRotation(GameEngine& gameContext, BombComponent& bombComp) const {
	VelocityComponent& bombVel = gameContext.entityMan->getComponent<VelocityComponent>(bombComp.id);
	SituationComponent& bombSit = gameContext.entityMan->getComponent<SituationComponent>(bombComp.id);

	if(bombVel.velocity.x != 0 || bombVel.velocity.y != 0)
		bombSit.rotation = Utils::getRoltationFromVector2(bombVel.velocity);
}