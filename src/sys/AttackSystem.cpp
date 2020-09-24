#include "AttackSystem.hpp"

#include <eng/GameEngine.hpp>
#include <iostream>

AttackSystem::AttackSystem() {}

AttackSystem::~AttackSystem() {}


void AttackSystem::update(GameEngine& gameContext) const {
	// Delete previous melee attacks
	deleteMeleeAttacks(gameContext);

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

	// FOR THE MELEE WEAPONS
	auto& meleeWeapons = gameContext.entityMan.getComponents<MeleeWeaponComponent>();
	float deltaTime = gameContext.getDeltaTime();

	for (auto& meleeWeap : meleeWeapons) {
		meleeWeap.cooldown += deltaTime;
	}

	// FOR THE DISTANCE WEAPONS
	auto& distanceWeapons = gameContext.entityMan.getComponents<DistanceWeaponComponent>();

	for (auto& distanceWeap : distanceWeapons) {
		distanceWeap.cooldown += deltaTime;
	}
}



void AttackSystem::checkPlayerAttacking(GameEngine& gameContext) const {
	InputComponent& playerInput = gameContext.entityMan.getComponent<InputComponent>(gameContext.playerId);

	if (playerInput.attacking) {
		SensorComponent& playerSensor = gameContext.entityMan.getComponent<SensorComponent>(gameContext.playerId);
		bool createMelee = false;
		//Decides if needs to use melee or distance weapon

		for (int sensoredId : playerSensor.entitiesSensoring) {
			EntityType sensoredType = gameContext.entityMan.getEntity(sensoredId).getType();

			if (sensoredType == EntityType::ENEMY) {
				createMelee = true;
			}
		}

		if (createMelee) {
			createMeleeAttack(gameContext, gameContext.entityMan.getComponent<MeleeWeaponComponent>(gameContext.playerId));
		}
		else {
			DistanceWeaponComponent& playerDistanceWeap = gameContext.entityMan.getComponent<DistanceWeaponComponent>(gameContext.playerId);
			VelocityComponent& playerVel = gameContext.entityMan.getComponent<VelocityComponent>(gameContext.playerId);
			SituationComponent& playerSit = gameContext.entityMan.getComponent<SituationComponent>(gameContext.playerId);

			playerDistanceWeap.attackVelX = playerDistanceWeap.attackGeneralVelociy;
			if (playerSit.facing == SituationComponent::Left) {
				playerDistanceWeap.attackVelX *= -1;
			}
			playerDistanceWeap.attackVelY = 0.f;
			if (playerInput.movingUp)
			{
				playerDistanceWeap.attackVelX = 0.f;
				playerDistanceWeap.attackVelY = -playerDistanceWeap.attackGeneralVelociy;
			}
			if (playerInput.movingDown)
			{
				if (playerVel.velocityY != 0) {
					// if the player is on air
					// shoot down
					playerDistanceWeap.attackVelX = 0.f;
					playerDistanceWeap.attackVelY = playerDistanceWeap.attackGeneralVelociy;
				}
			}
			createDistanceAttack(gameContext, playerDistanceWeap);
		}
		
	}
}

void AttackSystem::createMeleeAttack(GameEngine& gameContext, MeleeWeaponComponent& meleeAttacker) const {

	if (meleeAttacker.cooldown > meleeAttacker.maxCooldown) {
		SituationComponent& attackerSit = gameContext.entityMan.getComponent<SituationComponent>(meleeAttacker.id);

		GameObjectType attackGOtype = GameObjectType::MELEE_ATTACK;
		if (meleeAttacker.id == gameContext.playerId) {
			attackGOtype = GameObjectType::PLAYER_MELEE_ATTACK;
		}

		int attackId = gameContext.entityMan.createAttack(gameContext, attackerSit.x, attackerSit.y + 50, 0.f, attackGOtype);

		ColliderComponent& colliderComp = gameContext.entityMan.getComponent<ColliderComponent>(attackId);
		AttackComponent& attackComp     = gameContext.entityMan.getComponent<AttackComponent>(attackId);

		colliderComp.boundingRoot.bounding = meleeAttacker.attackBounding;
		attackComp.damage = meleeAttacker.damage;

		meleeAttacker.cooldown = 0.f;
	}
}

void AttackSystem::createDistanceAttack(GameEngine& gameContext, DistanceWeaponComponent& distanceWeaponAttacker) const{

	if (distanceWeaponAttacker.cooldown > distanceWeaponAttacker.maxCooldown) {
		SituationComponent& attackerSit = gameContext.entityMan.getComponent<SituationComponent>(distanceWeaponAttacker.id);

		GameObjectType attackGOtype = GameObjectType::DISTANCE_ATTACK;
		if (distanceWeaponAttacker.id == gameContext.playerId) {
			attackGOtype = GameObjectType::PLAYER_DISTANCE_ATTACK;
		}

		int attackId = gameContext.entityMan.createAttack(gameContext, attackerSit.x, attackerSit.y, 0.f, attackGOtype);

		ColliderComponent& colliderComp = gameContext.entityMan.getComponent<ColliderComponent>(attackId);
		AttackComponent& attackComp = gameContext.entityMan.getComponent<AttackComponent>(attackId);
		VelocityComponent& attackVel = gameContext.entityMan.getComponent<VelocityComponent>(attackId);

		colliderComp.boundingRoot.bounding = distanceWeaponAttacker.attackBounding;
		attackComp.damage = distanceWeaponAttacker.damage;
		attackVel.velocityX = distanceWeaponAttacker.attackVelX;
		attackVel.velocityY = distanceWeaponAttacker.attackVelY;
		attackVel.gravity = distanceWeaponAttacker.attackGravity;

		distanceWeaponAttacker.cooldown = 0.f;
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
