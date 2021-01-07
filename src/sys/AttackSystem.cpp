#include "AttackSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>
#include <iostream>
#include <algorithm>

AttackSystem::AttackSystem() {}

AttackSystem::~AttackSystem() {}


void AttackSystem::update(GameEngine& gameContext) const {
	// Delete previous melee attacks
	manageAttacks(gameContext);

	//manage COOLDOWN for all attacks
	addCooldownTimeToWeapons(gameContext);

	//manage attack creation
	checkPlayerAttacking(gameContext);
	checkEnemiesAttacking(gameContext);

	//manage hits
	checkAttacksHits(gameContext);
}

void AttackSystem::manageAttacks(GameEngine& gameContext) const { // this method delete attacks by time, and manage explosion animations and reset of the hitted enemies of damage platforms
	auto& attacks = gameContext.entityMan.getComponents<AttackComponent>();
	std::vector<int> attacksToDelete;
	attacksToDelete.reserve(attacks.size());

	for (AttackComponent& attack : attacks) {
		attack.lifetime += gameContext.getDeltaTime();

		if (attack.lifetime > attack.maxLifetime) {
			attacksToDelete.push_back(attack.id);
		}
		else {
			// manage the attack
			if (attack.type == AttackType::EXPLOSION && gameContext.entityMan.existsComponent<ExplosionAttackComponent>(attack.id)) {
				animateExplosion(gameContext, attack);
			}
			else if (attack.type == AttackType::DAMAGE_PLATFORM) {
				manageDamagePlatform(gameContext, attack);
			}
		}
	}

	//Delete attacks
	for (size_t i = 0; i < attacksToDelete.size(); ++i) {
		gameContext.eraseEntityByID(attacksToDelete[i]);
	}
}

void AttackSystem::animateExplosion(GameEngine& gameContext, AttackComponent& attack) const {
	ColliderComponent& attackCol = gameContext.entityMan.getComponent<ColliderComponent>(attack.id);
	ExplosionAttackComponent& explosionComp = gameContext.entityMan.getComponent<ExplosionAttackComponent>(attack.id);
	BoundingBox& attackBound = attackCol.boundingRoot.bounding;

	// Do bigger the collider
	attackBound.xLeft -= explosionComp.expansionVelocity;
	attackBound.xRight += explosionComp.expansionVelocity;
	attackBound.yDown += explosionComp.expansionVelocity;
	attackBound.yUp -= explosionComp.expansionVelocity;
}

void AttackSystem::manageDamagePlatform(GameEngine& gameContext, AttackComponent& attack) const {
	if (gameContext.entityMan.getComponent<ColliderComponent>(attack.id).collide) {
		// manages the time only if it is colliding
		attack.resetDamagedEntTimeCounter += gameContext.getDeltaTime();

		if (attack.resetDamagedEntTimeCounter > attack.resetDamagedEntTime) {
			attack.entitiesDamaged.clear();

			attack.resetDamagedEntTimeCounter = 0.f;
		}
	}
	else {
		attack.entitiesDamaged.clear();
		attack.resetDamagedEntTimeCounter = 0.f;
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
	InputComponent& playerInput = gameContext.entityMan.getComponent<InputComponent>(WorldElementsData::playerId);

	if (playerInput.attacking) {
		SensorComponent& playerSensor = gameContext.entityMan.getComponent<SensorComponent>(WorldElementsData::playerId);
		bool createMelee = false;
		//Decides if needs to use melee or distance weapon

		for (int sensoredId : playerSensor.entitiesSensoring) {
			EntityType sensoredType = gameContext.entityMan.getEntity(sensoredId).getType();

			if (sensoredType == EntityType::ENEMY) {
				createMelee = true;
			}
		}

		if (createMelee) {
			createMeleeAttack(gameContext, gameContext.entityMan.getComponent<MeleeWeaponComponent>(WorldElementsData::playerId));

			// Reset the distance cooldown too
			if (gameContext.entityMan.existsComponent<DistanceWeaponComponent>(WorldElementsData::playerId)) {
				gameContext.entityMan.getComponent<DistanceWeaponComponent>(WorldElementsData::playerId).cooldown = 0.f;
			}
		}
		else {
			DistanceWeaponComponent& playerDistanceWeap = gameContext.entityMan.getComponent<DistanceWeaponComponent>(WorldElementsData::playerId);
			VelocityComponent& playerVel  = gameContext.entityMan.getComponent<VelocityComponent>(WorldElementsData::playerId);
			SituationComponent& playerSit = gameContext.entityMan.getComponent<SituationComponent>(WorldElementsData::playerId);

			playerDistanceWeap.attackVel.y = 0.f;
			playerDistanceWeap.attackVel.x = playerDistanceWeap.attackGeneralVelociy;
			if (playerInput.movingUp)
			{
				playerDistanceWeap.attackVel.x = 0.f;
				playerDistanceWeap.attackVel.y = -playerDistanceWeap.attackGeneralVelociy;
			}
			if (playerInput.movingDown)
			{
				if (playerVel.velocity.y != 0) {
					// if the player is on air
					// shoot down
					playerDistanceWeap.attackVel.x = 0.f;
					playerDistanceWeap.attackVel.y = playerDistanceWeap.attackGeneralVelociy;
				}
			}

			if (playerSit.facing == SituationComponent::Left) {
				playerDistanceWeap.attackVel.x *= -1;
			}

			bool attackCreated = createDistanceAttack(gameContext, playerDistanceWeap);

			// To update the ammo
			if (attackCreated && !playerDistanceWeap.infiniteAmmo) {
				--playerDistanceWeap.ammo;

				if (playerDistanceWeap.ammo == 0) {
					//Delete this weapon and set the normal pistol
					gameContext.entityMan.eraseComponent<DistanceWeaponComponent>(WorldElementsData::playerId);

					Utils::setNormalPistolToEntity(gameContext, WorldElementsData::playerId);
				}
			}

			/*// Reset the melee cooldown too
			if (gameContext.entityMan.existsComponent<MeleeWeaponComponent>(WorldElementsData::playerId)) {
				gameContext.entityMan.getComponent<MeleeWeaponComponent>(WorldElementsData::playerId).cooldown = 0.f;
			}*/
		}
		
	}
}

void AttackSystem::checkEnemiesAttacking(GameEngine& gameContext) const {
	// For the melee attacks
	auto& AIMeleeAtkComponents = gameContext.entityMan.getComponents<AIMeleeAtkComponent>();

	for (AIMeleeAtkComponent& AIMeleeComp : AIMeleeAtkComponents) {
		if (AIMeleeComp.createAttack) {
			createMeleeAttack(gameContext, gameContext.entityMan.getComponent<MeleeWeaponComponent>(AIMeleeComp.id));
		}
	}


	// For the distance attacks
	auto& AIDistanceAtkComponents = gameContext.entityMan.getComponents<AIDistanceAtkComponent>();

	for (AIDistanceAtkComponent& AIDistanceComp : AIDistanceAtkComponents) {
		if (AIDistanceComp.createAttack) {
			createDistanceAttack(gameContext, gameContext.entityMan.getComponent<DistanceWeaponComponent>(AIDistanceComp.id));
		}
	}


	// For the drop bomb enemies
	auto& AIDropBombComponents = gameContext.entityMan.getComponents<AIDropBombComponent>();

	for (AIDropBombComponent& AIDropBombComp : AIDropBombComponents) {
		if (AIDropBombComp.createBomb) {
			createDistanceAttack(gameContext, gameContext.entityMan.getComponent<DistanceWeaponComponent>(AIDropBombComp.id));
		}
	}
}



bool AttackSystem::createMeleeAttack(GameEngine& gameContext, MeleeWeaponComponent& meleeAttacker) const {

	if (meleeAttacker.cooldown > meleeAttacker.maxCooldown) {
		SituationComponent& attackerSit = gameContext.entityMan.getComponent<SituationComponent>(meleeAttacker.id);
		Vector2 attackPos = Vector2(attackerSit.position.x, attackerSit.position.y);

		GameObjectType attackGOtype = GameObjectType::MELEE_ATTACK;
		if (meleeAttacker.id == WorldElementsData::playerId) {
			attackGOtype = GameObjectType::PLAYER_MELEE_ATTACK;
		}

		// Calculate attack spawn situation outside of the attacker collidable
		if (gameContext.entityMan.existsComponent<ColliderComponent>(attackerSit.id)) {
			BoundingBox& attackerBounding = gameContext.entityMan.getComponent<ColliderComponent>(attackerSit.id).boundingRoot.bounding;

			if (attackerSit.facing == SituationComponent::Right) {
				attackPos.x += attackerBounding.xRight;
			}
			else {
				// Left
				attackPos.x -= (meleeAttacker.attackBounding.xRight - meleeAttacker.attackBounding.xLeft);
			}
		}


		int attackId = gameContext.entityMan.createAttack(gameContext, attackPos, 0.f, attackGOtype);

		ColliderComponent& colliderComp = gameContext.entityMan.getComponent<ColliderComponent>(attackId);
		AttackComponent& attackComp     = gameContext.entityMan.getComponent<AttackComponent>(attackId);

		colliderComp.boundingRoot.bounding = meleeAttacker.attackBounding;
		attackComp.damage = meleeAttacker.damage;
		attackComp.maxLifetime = meleeAttacker.attackLifetime;

		meleeAttacker.cooldown = 0.f;

		// Play attack sound
		gameContext.getSoundFacadeRef().loadSound(meleeAttacker.attackSound.soundPath);
		gameContext.getSoundFacadeRef().playSound(meleeAttacker.attackSound);

		return true;
	}

	return false;
}



bool AttackSystem::createDistanceAttack(GameEngine& gameContext, DistanceWeaponComponent& distanceWeaponAttacker) const {
	// Chooses the type of disatance attack that has to be generated
	if (distanceWeaponAttacker.cooldown > distanceWeaponAttacker.maxCooldown) {

		switch (distanceWeaponAttacker.attackGeneratedType)
		{
		case DistanceWeaponComponent::BULLET:
			createBulletAttack(gameContext, distanceWeaponAttacker);
			break;

		case DistanceWeaponComponent::BOMB:
			createBombEntity(gameContext, distanceWeaponAttacker);
			break;

		case DistanceWeaponComponent::LASER:
			createLaserAttack(gameContext, distanceWeaponAttacker);
			break;
		}

		return true;
	}

	return false;
}
void AttackSystem::createBulletAttack(GameEngine& gameContext, DistanceWeaponComponent& distanceWeaponAttacker) const{

	SituationComponent& attackerSit = gameContext.entityMan.getComponent<SituationComponent>(distanceWeaponAttacker.id);

	GameObjectType attackGOtype = GameObjectType::DISTANCE_ATTACK;
	if (distanceWeaponAttacker.id == WorldElementsData::playerId) {
		attackGOtype = GameObjectType::PLAYER_DISTANCE_ATTACK;
	}

	int attackId = gameContext.entityMan.createAttack(gameContext, attackerSit.position, 0.f, attackGOtype);

	ColliderComponent& colliderComp = gameContext.entityMan.getComponent<ColliderComponent>(attackId);
	AttackComponent& attackComp = gameContext.entityMan.getComponent<AttackComponent>(attackId);
	VelocityComponent& attackVel = gameContext.entityMan.getComponent<VelocityComponent>(attackId);

	colliderComp.boundingRoot.bounding = distanceWeaponAttacker.attackBounding;
	attackComp.damage = distanceWeaponAttacker.damage;
	attackComp.maxLifetime = distanceWeaponAttacker.attackLifetime;
	attackVel.velocity = distanceWeaponAttacker.attackVel;
	attackVel.gravity = distanceWeaponAttacker.attackGravity;


	distanceWeaponAttacker.cooldown = 0.f;

	// Play shoot sound
	gameContext.getSoundFacadeRef().loadSound(distanceWeaponAttacker.attackSound.soundPath);
	gameContext.getSoundFacadeRef().playSound(distanceWeaponAttacker.attackSound);
}

void AttackSystem::createBombEntity(GameEngine& gameContext, DistanceWeaponComponent& distanceWeaponAttacker) const {
	SituationComponent& attackerSit = gameContext.entityMan.getComponent<SituationComponent>(distanceWeaponAttacker.id);

	GameObjectType attackGOtype = GameObjectType::BOMB;
	if (distanceWeaponAttacker.id == WorldElementsData::playerId) {
		attackGOtype = GameObjectType::PLAYER_BOMB;
	}

	int bombId = gameContext.entityMan.createBomb(gameContext, attackerSit.position, 0.f, attackGOtype);

	ColliderComponent& colliderComp = gameContext.entityMan.getComponent<ColliderComponent>(bombId);
	VelocityComponent& bombVel = gameContext.entityMan.getComponent<VelocityComponent>(bombId);
	BombComponent& bombComp = gameContext.entityMan.getComponent<BombComponent>(bombId);

	colliderComp.boundingRoot.bounding = distanceWeaponAttacker.attackBounding;

	bombComp.damageExplosion = distanceWeaponAttacker.damage;
	bombComp.explosionLifetime = distanceWeaponAttacker.attackLifetime;
	bombComp.activated = distanceWeaponAttacker.startActivated;
	bombComp.explosionTime = distanceWeaponAttacker.explosionTime;
	bombComp.explosionExpansion = distanceWeaponAttacker.explosionExpansion;

	bombVel.velocity = distanceWeaponAttacker.attackVel;
	bombVel.gravity = distanceWeaponAttacker.attackGravity;


	distanceWeaponAttacker.cooldown = 0.f;

	// Play shoot sound
	gameContext.getSoundFacadeRef().loadSound(distanceWeaponAttacker.attackSound.soundPath);
	gameContext.getSoundFacadeRef().playSound(distanceWeaponAttacker.attackSound);
}

void AttackSystem::createLaserAttack(GameEngine& gameContext, DistanceWeaponComponent& distanceWeaponAttacker) const {

	SituationComponent& attackerSit = gameContext.entityMan.getComponent<SituationComponent>(distanceWeaponAttacker.id);

	GameObjectType attackGOtype = GameObjectType::LASER;
	if (distanceWeaponAttacker.id == WorldElementsData::playerId) {
		attackGOtype = GameObjectType::PLAYER_LASER;
	}

	int attackId = gameContext.entityMan.createAttack(gameContext, attackerSit.position, 0.f, attackGOtype);

	ColliderComponent& colliderComp = gameContext.entityMan.getComponent<ColliderComponent>(attackId);
	AttackComponent& attackComp = gameContext.entityMan.getComponent<AttackComponent>(attackId);
	VelocityComponent& attackVel = gameContext.entityMan.getComponent<VelocityComponent>(attackId);

	attackComp.damage = distanceWeaponAttacker.damage;
	attackComp.maxLifetime = distanceWeaponAttacker.attackLifetime;
	attackVel.velocity = distanceWeaponAttacker.attackVel;
	attackVel.gravity = distanceWeaponAttacker.attackGravity;


	// For setting the laser bounding
	colliderComp.boundingRoot.bounding = distanceWeaponAttacker.attackBounding;

	if (attackerSit.facing == SituationComponent::Left) {
		colliderComp.boundingRoot.bounding.xRight = distanceWeaponAttacker.attackBounding.xLeft;
		colliderComp.boundingRoot.bounding.xLeft = distanceWeaponAttacker.attackBounding.xLeft - (distanceWeaponAttacker.attackBounding.xRight - distanceWeaponAttacker.attackBounding.xLeft);
	}
	
	if (distanceWeaponAttacker.id == WorldElementsData::playerId) {
		VelocityComponent& playerVel = gameContext.entityMan.getComponent<VelocityComponent>(WorldElementsData::playerId);
		InputComponent& playerInput = gameContext.entityMan.getComponent<InputComponent>(WorldElementsData::playerId);

		if (playerInput.movingUp)
		{
			colliderComp.boundingRoot.bounding.xLeft  = distanceWeaponAttacker.attackBounding.yUp;
			colliderComp.boundingRoot.bounding.xRight = distanceWeaponAttacker.attackBounding.yDown;
			colliderComp.boundingRoot.bounding.yUp    = distanceWeaponAttacker.attackBounding.xLeft - (distanceWeaponAttacker.attackBounding.xRight - distanceWeaponAttacker.attackBounding.xLeft);
			colliderComp.boundingRoot.bounding.yDown  = distanceWeaponAttacker.attackBounding.xLeft;
		}
		if (playerInput.movingDown)
		{
			if (playerVel.velocity.y != 0) {
				// if the player is on air
				// shoot down
				colliderComp.boundingRoot.bounding.xLeft = distanceWeaponAttacker.attackBounding.yUp;
				colliderComp.boundingRoot.bounding.xRight = distanceWeaponAttacker.attackBounding.yDown;
				colliderComp.boundingRoot.bounding.yUp = distanceWeaponAttacker.attackBounding.xLeft;
				colliderComp.boundingRoot.bounding.yDown = distanceWeaponAttacker.attackBounding.xRight;
			}
		}
	}
	

	distanceWeaponAttacker.cooldown = 0.f;

	// Play shoot sound
	gameContext.getSoundFacadeRef().loadSound(distanceWeaponAttacker.attackSound.soundPath);
	gameContext.getSoundFacadeRef().playSound(distanceWeaponAttacker.attackSound);
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

		if (!(std::find(attack.entitiesDamaged.begin(), attack.entitiesDamaged.end(), entityHitId) != attack.entitiesDamaged.end())) { //if this entity isn't already hitted
			damageEntity(gameContext, attack, entityHitId);
		}
	}

	// After hit, delete attack if is a distance attack
	if(attack.type == AttackType::DISTANCE)
		attacksToDelete.push_back(attack.id);
}

void AttackSystem::damageEntity(GameEngine& gameContext, AttackComponent& attack, int entityHitId) const {

	if (gameContext.entityMan.existsComponent<HealthComponent>(entityHitId)) { // if the hitted entity has health
		HealthComponent& hittedHealth = gameContext.entityMan.getComponent<HealthComponent>(entityHitId);

		hittedHealth.damageReceived += attack.damage;
		hittedHealth.damaged = true;
	}

	attack.entitiesDamaged.push_back(entityHitId);
}
