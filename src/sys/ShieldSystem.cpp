#include "ShieldSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>
#include <iostream>

ShieldSystem::ShieldSystem() {}

ShieldSystem::~ShieldSystem() {}


void ShieldSystem::update(GameEngine& gameContext) const {
	auto& shieldComponents = gameContext.entityMan.getComponents<ShieldComponent>();

	for (ShieldComponent& shield : shieldComponents) {
		if (gameContext.entityMan.existsComponent<SituationComponent>(shield.objectiveId) && gameContext.entityMan.existsComponent<ColliderComponent>(shield.objectiveId)) {
			checkIfObjLosesHealth(gameContext, shield);
			//updateShieldCollider(gameContext, shield); 
			setInObjectivePosition(gameContext, shield);
			checkEnemyHits(gameContext, shield);
		}
	}
}


void ShieldSystem::setInObjectivePosition(GameEngine& gameContext, ShieldComponent& shield) const {
	SituationComponent& shieldSit    = gameContext.entityMan.getComponent<SituationComponent>(shield.id);
	SituationComponent& objectiveSit = gameContext.entityMan.getComponent<SituationComponent>(shield.objectiveId);
	ColliderComponent& objectiveColl = gameContext.entityMan.getComponent<ColliderComponent>(shield.objectiveId);

	// Calculate center of objective
	Vector2 objCenterWorld = Utils::getCenterOfBounding(objectiveColl.boundingRoot.bounding);

	shieldSit.position.x = objectiveSit.position.x - ( shield.center.x - objCenterWorld.x);
	shieldSit.position.y = objectiveSit.position.y - ( shield.center.y - objCenterWorld.y);
}

/*void ShieldSystem::updateShieldCollider(GameEngine& gameContext, ShieldComponent& shield) const {

}*/

void ShieldSystem::checkEnemyHits(GameEngine& gameContext, ShieldComponent& shield) const {
	ColliderComponent& shieldColl = gameContext.entityMan.getComponent<ColliderComponent>(shield.id);

	if (gameContext.entityMan.existsComponent<MeleeWeaponComponent>(shield.id)) {
		MeleeWeaponComponent& shieldWeapon = gameContext.entityMan.getComponent<MeleeWeaponComponent>(shield.id);


		if (shieldWeapon.cooldown > shieldWeapon.maxCooldown) {
			// Damages the entities that have health component and hit the shield
			bool hitEntity = damageHittedEntities(gameContext, shieldColl.boundingRoot, shieldWeapon);

			if (hitEntity) {
				shieldWeapon.cooldown = 0.f; // reset cooldown if has done dmage to entities

				// Play attack sound
				gameContext.getSoundFacadeRef().loadSound(shieldWeapon.attackSound.soundPath);
				gameContext.getSoundFacadeRef().playSound(shieldWeapon.attackSound);
			}
		}
	}
}

bool ShieldSystem::damageHittedEntities(GameEngine& gameContext, BoundingBoxNode& boundingNode, MeleeWeaponComponent& shieldWeapon) const {
	bool hitEntity = false;

	for (int hittedEntID : boundingNode.bounding.entitiesColliding) {
		if (gameContext.entityMan.existsComponent<HealthComponent>(hittedEntID)) {
			HealthComponent& hittedHealth = gameContext.entityMan.getComponent<HealthComponent>(hittedEntID);

			hittedHealth.damaged = true;
			hittedHealth.damageReceived += shieldWeapon.damage;

			hitEntity = true;
		}
	}


	for (auto& c : boundingNode.childs) {
		if (damageHittedEntities(gameContext, c, shieldWeapon) || hitEntity) {
			hitEntity = true;
		}
	}

	return hitEntity;
}


void ShieldSystem::checkIfObjLosesHealth(GameEngine& gameContext, ShieldComponent& shield) const {
	HealthComponent& objHealth = gameContext.entityMan.getComponent<HealthComponent>(shield.objectiveId);

	if (objHealth.damaged) {
		objHealth.damaged = false;
		objHealth.damageReceived = 0;
	}
}