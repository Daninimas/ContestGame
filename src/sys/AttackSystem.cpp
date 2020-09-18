#include "AttackSystem.hpp"

#include <eng/GameEngine.hpp>
#include <iostream>

AttackSystem::AttackSystem() {}

AttackSystem::~AttackSystem() {}


void AttackSystem::update(GameEngine& gameContext) const {
	// Delete previous melee attacks
	//deleteMeleeAttacks(gameContext);

	//manage COOLDOWN for all attacks
	addCooldownTimeToWeapons(gameContext);

	checkPlayerAttacking(gameContext);

	// Manage hits
	checkAttacksHits(gameContext);
}

void AttackSystem::deleteMeleeAttacks(GameEngine& gameContext) const {
	auto& attacks = gameContext.entityMan.getComponents<AttackComponent>();

	for (AttackComponent& attack : attacks) {
		if (attack.type == AttackType::MELEE) {
			gameContext.eraseEntityByID(attack.id);
		}
	}
}


void AttackSystem::addCooldownTimeToWeapons(GameEngine& gameContext) const {
	auto& meleeWeapons = gameContext.entityMan.getComponents<MeleeWeaponComponent>();
	float deltaTime = gameContext.getDeltaTime();

	for (auto& meleeWeap : meleeWeapons) {
		meleeWeap.cooldown += deltaTime;
	}
}



void AttackSystem::checkPlayerAttacking(GameEngine& gameContext) const {
	InputComponent& playerInput = gameContext.entityMan.getComponent<InputComponent>(gameContext.playerId);


	if (playerInput.attacking) {
		// TODO attack mele or distance
		createMeleeAttack(gameContext, gameContext.playerId);
	}
}

void AttackSystem::createMeleeAttack(GameEngine& gameContext, int attackerID) const {
	MeleeWeaponComponent& meleeWeapon = gameContext.entityMan.getComponent<MeleeWeaponComponent>(attackerID);

	if (meleeWeapon.cooldown > meleeWeapon.maxCooldown) {
		SituationComponent& attackerSit = gameContext.entityMan.getComponent<SituationComponent>(attackerID);

		int attackId = gameContext.entityMan.createAttack(gameContext, attackerSit.x, attackerSit.y+50, 0.f, GameObjectType::PLAYER_MELEE_ATTACK);

		meleeWeapon.cooldown = 0.f;
	}
}


void AttackSystem::checkAttacksHits(GameEngine& gameContext) const{
	auto& attacks = gameContext.entityMan.getComponents<AttackComponent>();
	std::vector<int> attacksToDelete;
	attacksToDelete.reserve(attacks.size());

	for (AttackComponent& attack : attacks) {
		ColliderComponent& attackCol = gameContext.entityMan.getComponent<ColliderComponent>(attack.id);
		
		if (attackCol.collide) {
			resolveAttackHit(gameContext, attackCol, attack, attacksToDelete);
		}
	}

	//Delete collided attacks
	for (size_t i = 0; i < attacksToDelete.size(); ++i) {
		gameContext.eraseEntityByID(attacksToDelete[i]);
	}
}

void AttackSystem::resolveAttackHit(GameEngine& gameContext, ColliderComponent& attackCol, AttackComponent& attack, std::vector<int>& attacksToDelete) const {
	// I assume that the attack will only have one boundingBox
	std::vector<int>& entitiesColliding = attackCol.boundingRoot.bounding.entitiesColliding;

	for (int entityHitId : entitiesColliding) {
		damageEntity(gameContext, attack, entityHitId);
	}

	// After hit, delete attack
	attacksToDelete.push_back(attack.id);
}

void AttackSystem::damageEntity(GameEngine& gameContext, AttackComponent& attack, int entityHitId) const {

	if (gameContext.entityMan.existsComponent<HeathComponent>(entityHitId)) { // if the hitted entity has health
		HeathComponent& hittedHealth = gameContext.entityMan.getComponent<HeathComponent>(entityHitId);

		hittedHealth.damageReceived += attack.damage;
		hittedHealth.damaged = true;
	}
}
