#include "BombSystem.hpp"

#include <eng/GameEngine.hpp>
#include <enum/AttackType.hpp>
#include <iostream>

BombSystem::BombSystem() {}

BombSystem::~BombSystem() {}

void BombSystem::update(GameEngine& gameContext) const {
	auto& BombComponents = gameContext.entityMan.getComponents<BombComponent>();
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
	ColliderComponent& collider = gameContext.entityMan.getComponent<ColliderComponent>(bombComp.id);

	bombComp.activated = collider.collide;
}

void BombSystem::explodeBomb(GameEngine& gameContext, int bombId) const {
	SituationComponent& bombSit   = gameContext.entityMan.getComponent<SituationComponent>(bombId);
	BombComponent& bombComp       = gameContext.entityMan.getComponent<BombComponent>(bombId);
	ColliderComponent& bombColl   = gameContext.entityMan.getComponent<ColliderComponent>(bombId);

	// Creates the explosion attack entity
	int explosionId = gameContext.entityMan.createAttack(gameContext, bombSit.x, bombSit.y, bombSit.rotation, GameObjectType::EXPLOSION);

	ExplosionAttackComponent& explosionComp = gameContext.entityMan.getComponent<ExplosionAttackComponent>(explosionId);
	ColliderComponent& explosionCollider    = gameContext.entityMan.getComponent<ColliderComponent>(explosionId);
	AttackComponent& explosionAttack        = gameContext.entityMan.getComponent<AttackComponent>(explosionId);

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