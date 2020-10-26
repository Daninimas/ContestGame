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
	checkEnemiesAttacking(gameContext);

	// Manage hits
	checkAttacksHits(gameContext);
}

void AttackSystem::deleteMeleeAttacks(GameEngine& gameContext) const {
	auto& attacks = gameContext.entityMan.getComponents<AttackComponent>();
	std::vector<int> attacksToDelete;
	attacksToDelete.reserve(attacks.size());

	for (AttackComponent& attack : attacks) {
		if (attack.type == AttackType::MELEE) {
			--attack.attackLifetime;

			if (attack.attackLifetime == 0)
				attacksToDelete.push_back(attack.id);
		}
	}

	//Delete collided attacks
	for (size_t i = 0; i < attacksToDelete.size(); ++i) {
		gameContext.eraseEntityByID(attacksToDelete[i]);
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
	InputComponent& playerInput = gameContext.entityMan.getComponent<InputComponent>(WorldData::playerId);

	if (playerInput.attacking) {
		SensorComponent& playerSensor = gameContext.entityMan.getComponent<SensorComponent>(WorldData::playerId);
		bool createMelee = false;
		//Decides if needs to use melee or distance weapon

		for (int sensoredId : playerSensor.entitiesSensoring) {
			EntityType sensoredType = gameContext.entityMan.getEntity(sensoredId).getType();

			if (sensoredType == EntityType::ENEMY) {
				createMelee = true;
			}
		}

		if (createMelee) {
			createMeleeAttack(gameContext, gameContext.entityMan.getComponent<MeleeWeaponComponent>(WorldData::playerId));

			// Reset the distance cooldown too
			if (gameContext.entityMan.existsComponent<DistanceWeaponComponent>(WorldData::playerId)) {
				gameContext.entityMan.getComponent<DistanceWeaponComponent>(WorldData::playerId).cooldown = 0.f;
			}
		}
		else {
			DistanceWeaponComponent& playerDistanceWeap = gameContext.entityMan.getComponent<DistanceWeaponComponent>(WorldData::playerId);
			VelocityComponent& playerVel = gameContext.entityMan.getComponent<VelocityComponent>(WorldData::playerId);

			playerDistanceWeap.attackVelY = 0.f;
			playerDistanceWeap.attackVelX = playerDistanceWeap.attackGeneralVelociy;
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

			/*// Reset the melee cooldown too
			if (gameContext.entityMan.existsComponent<MeleeWeaponComponent>(WorldData::playerId)) {
				gameContext.entityMan.getComponent<MeleeWeaponComponent>(WorldData::playerId).cooldown = 0.f;
			}*/
		}
		
	}
}

void AttackSystem::checkEnemiesAttacking(GameEngine& gameContext) const {
	auto& AIMeleeComponents = gameContext.entityMan.getComponents<AIMeleeComponent>();

	for (AIMeleeComponent& AIMeleeComp : AIMeleeComponents) {
		if (AIMeleeComp.createAttack) {
			createMeleeAttack(gameContext, gameContext.entityMan.getComponent<MeleeWeaponComponent>(AIMeleeComp.id));
		}
	}
}



void AttackSystem::createMeleeAttack(GameEngine& gameContext, MeleeWeaponComponent& meleeAttacker) const {

	if (meleeAttacker.cooldown > meleeAttacker.maxCooldown) {
		SituationComponent& attackerSit = gameContext.entityMan.getComponent<SituationComponent>(meleeAttacker.id);
		float attackX = attackerSit.x;
		float attackY = attackerSit.y;

		GameObjectType attackGOtype = GameObjectType::MELEE_ATTACK;
		if (meleeAttacker.id == WorldData::playerId) {
			attackGOtype = GameObjectType::PLAYER_MELEE_ATTACK;
		}

		// Calculate attack spawn situation outside of the attacker collidable
		if (gameContext.entityMan.existsComponent<ColliderComponent>(attackerSit.id)) {
			BoundingBox& attackerBounding = gameContext.entityMan.getComponent<ColliderComponent>(attackerSit.id).boundingRoot.bounding;

			if (attackerSit.facing == SituationComponent::Right) {
				attackX += attackerBounding.xRight;
			}
			else {
				// Left
				attackX -= (meleeAttacker.attackBounding.xRight - meleeAttacker.attackBounding.xLeft);
			}
		}


		int attackId = gameContext.entityMan.createAttack(gameContext, attackX, attackY, 0.f, attackGOtype);

		ColliderComponent& colliderComp = gameContext.entityMan.getComponent<ColliderComponent>(attackId);
		AttackComponent& attackComp     = gameContext.entityMan.getComponent<AttackComponent>(attackId);

		colliderComp.boundingRoot.bounding = meleeAttacker.attackBounding;
		attackComp.damage = meleeAttacker.damage;

		meleeAttacker.cooldown = 0.f;

		// Play attack sound
		gameContext.getSoundFacadeRef().loadSound(meleeAttacker.attackSound.soundPath);
		gameContext.getSoundFacadeRef().playSound(meleeAttacker.attackSound);
	}
}

void AttackSystem::createDistanceAttack(GameEngine& gameContext, DistanceWeaponComponent& distanceWeaponAttacker) const{

	if (distanceWeaponAttacker.cooldown > distanceWeaponAttacker.maxCooldown) {
		SituationComponent& attackerSit = gameContext.entityMan.getComponent<SituationComponent>(distanceWeaponAttacker.id);

		if (attackerSit.facing == SituationComponent::Left) {
			distanceWeaponAttacker.attackVelX *= -1;
		}

		GameObjectType attackGOtype = GameObjectType::DISTANCE_ATTACK;
		if (distanceWeaponAttacker.id == WorldData::playerId) {
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

		// Play shoot sound
		gameContext.getSoundFacadeRef().loadSound(distanceWeaponAttacker.attackSound.soundPath);
		gameContext.getSoundFacadeRef().playSound(distanceWeaponAttacker.attackSound);
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
