#include "BombSystem.hpp"

#include <eng/GameEngine.hpp>
#include <enum/AttackType.hpp>
#include <iostream>

BombSystem::BombSystem() {}

BombSystem::~BombSystem() {}

void BombSystem::update(GameEngine& gameContext) const {
	auto& BombWeaponComponents = gameContext.entityMan.getComponents<BombWeaponComponent>();
	std::vector<int> bombsToExplode;
	bombsToExplode.reserve(BombWeaponComponents.size());

	for (BombWeaponComponent& bombComp : BombWeaponComponents) {
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


void BombSystem::updateBombTimers(GameEngine& gameContext, BombWeaponComponent& bombComp, std::vector<int>& bombsToExplode) const {
	bombComp.currentTime += gameContext.getDeltaTime();

	if (bombComp.currentTime > bombComp.explosionTime) {
		bombsToExplode.push_back(bombComp.id);
	}
}

void BombSystem::checkBombCollision(GameEngine& gameContext, BombWeaponComponent& bombComp) const {
	ColliderComponent& collider = gameContext.entityMan.getComponent<ColliderComponent>(bombComp.id);

	bombComp.activated = collider.collide;
}

void BombSystem::explodeBomb(GameEngine& gameContext, int bombId) const {
	SituationComponent& bombSit   = gameContext.entityMan.getComponent<SituationComponent>(bombId);
	BombWeaponComponent& bombWeap = gameContext.entityMan.getComponent<BombWeaponComponent>(bombId);
	ColliderComponent& bombColl   = gameContext.entityMan.getComponent<ColliderComponent>(bombId);

	// Creates the explosion attack entity
	int explosionId = gameContext.entityMan.createAttack(gameContext, bombSit.x, bombSit.y, bombSit.rotation, GameObjectType::EXPLOSION);

	ExplosionAttackComponent& explosionComp = gameContext.entityMan.getComponent<ExplosionAttackComponent>(explosionId);
	ColliderComponent& explosionCollider    = gameContext.entityMan.getComponent<ColliderComponent>(explosionId);
	AttackComponent& explosionAttack        = gameContext.entityMan.getComponent<AttackComponent>(explosionId);

	// ExplosionAttackComponent
	explosionComp.expansionVelocity = bombWeap.explosionExpansion;

	// ColliderComponent
	BoundingBox& bombBounding = bombColl.boundingRoot.bounding;
	explosionCollider.boundingRoot.bounding = {bombBounding.xLeft, bombBounding.xRight, bombBounding.xLeft, bombBounding.xRight}; // To make it square
	explosionCollider.collisionLayer	    = bombColl.collisionLayer;
	explosionCollider.layerMasc             = bombColl.layerMasc;

	// AttackComponent
	explosionAttack.damage		= bombWeap.damage;
	explosionAttack.maxLifetime = bombWeap.explosionLifetime;
}