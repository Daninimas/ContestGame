#include "OrbitalWeaponSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>
#include <iostream>

OrbitalWeaponSystem::OrbitalWeaponSystem() {}

OrbitalWeaponSystem::~OrbitalWeaponSystem() {}


void OrbitalWeaponSystem::update(GameEngine& gameContext) const {
	addCooldownToOrbitalWeapons(gameContext);

	checkEnemiesAttacking(gameContext);

	manageOrbitalStrikes(gameContext);
}

void OrbitalWeaponSystem::addCooldownToOrbitalWeapons(GameEngine& gameContext) const {
	float deltaTime = gameContext.getDeltaTime();
	auto& orbitalWeapons = gameContext.entityMan->getComponents<OrbitalWeaponComponent>();

	for (auto& orbitalWeap : orbitalWeapons) {
		orbitalWeap.cooldown += deltaTime;
	}
}


void OrbitalWeaponSystem::checkEnemiesAttacking(GameEngine& gameContext) const {
	auto& AIOrbitalStrikers = gameContext.entityMan->getComponents<AIOrbitalAtkComponent>();

	for (AIOrbitalAtkComponent& AIOrbitalStriker : AIOrbitalStrikers) {
		OrbitalWeaponComponent& orbitalWeapon = gameContext.entityMan->getComponent<OrbitalWeaponComponent>(AIOrbitalStriker.id);

		if (orbitalWeapon.cooldown > orbitalWeapon.maxCooldown && gameContext.entityMan->existsComponent<SituationComponent>(AIOrbitalStriker.objectiveId)) {
			generateOrbitalMarker(gameContext, orbitalWeapon, gameContext.entityMan->getComponent<SituationComponent>(AIOrbitalStriker.objectiveId));
		}
	}
}

void OrbitalWeaponSystem::generateOrbitalMarker(GameEngine& gameContext, OrbitalWeaponComponent& orbitalWeapon, SituationComponent& objectiveSituation) const {
	// Set the position of the orbital attack
	Vector2 objectiveCenter = Utils::getCenterOfBounding(gameContext.entityMan->getComponent<ColliderComponent>(objectiveSituation.id).boundingRoot.bounding);
	Vector2 markerCenter = Utils::getCenterOfBounding(orbitalWeapon.markerBounding);
	orbitalWeapon.attackPosition.x = objectiveSituation.position.x + objectiveCenter.x;
	orbitalWeapon.attackPosition.y = 0.f;

	//std::cout << "Pos: ( " << (orbitalWeapon.attackPosition.x - markerCenter.x) << ", " << orbitalWeapon.attackPosition.y << ")\n";
	//std::cout << "Objective Pos: ( " << objectiveSituation.position.x << ", " << objectiveSituation.position.y << ") id: " << objectiveSituation.id << " playerID: " << WorldElementsData::playerId <<"\n";

	int markerID = gameContext.entityMan->createOrbitalMarker(gameContext, { orbitalWeapon.attackPosition.x-markerCenter.x, orbitalWeapon.attackPosition.y }, GameObjectType::ORBITAL_MARKER);

	AutodeleteComponent& autodeleteComp = gameContext.entityMan->getComponent<AutodeleteComponent>(markerID);
	RenderComponent& renderComp = gameContext.entityMan->getComponent<RenderComponent>(markerID);

	calculateBoundingToFloor(gameContext, objectiveSituation, orbitalWeapon);

	autodeleteComp.timeToDelete = orbitalWeapon.generateAttackTime;

	renderComp.spriteRect = orbitalWeapon.attackBounding; // The attack bounding previous calculated with the distance to the floor
	renderComp.spriteRect.xLeft = orbitalWeapon.markerBounding.xLeft;
	renderComp.spriteRect.xRight = orbitalWeapon.markerBounding.xRight;

	// Set the counters for the attack
	orbitalWeapon.activated = true;
	orbitalWeapon.cooldown = 0.f;
	orbitalWeapon.generateAttackTimeCounter = 0.f;

	// Update render
	gameContext.entityMan->addEntityToUpdate(markerID);

	// Play orbital marker sound
	gameContext.getSoundFacadeRef().loadSound(orbitalWeapon.markerSound.soundPath);
	gameContext.getSoundFacadeRef().playSound(orbitalWeapon.markerSound);
}




void OrbitalWeaponSystem::manageOrbitalStrikes(GameEngine& gameContext) const {
	auto& orbitalWeapons = gameContext.entityMan->getComponents<OrbitalWeaponComponent>();
	float deltaTime = gameContext.getDeltaTime();

	for (OrbitalWeaponComponent& orbitalWeapon : orbitalWeapons) {
		if (orbitalWeapon.activated) {
			orbitalWeapon.generateAttackTimeCounter += deltaTime;

			if (orbitalWeapon.generateAttackTimeCounter > orbitalWeapon.generateAttackTime) {
				generateOrbitalAttack(gameContext, orbitalWeapon);
			}
		}
	}
}



void OrbitalWeaponSystem::generateOrbitalAttack(GameEngine& gameContext, OrbitalWeaponComponent& orbitalWeapon) const {
	GameObjectType attackGOtype = GameObjectType::MELEE_ATTACK;
	if (orbitalWeapon.id == WorldElementsData::playerId) {
		attackGOtype = GameObjectType::PLAYER_MELEE_ATTACK;
	}

	Vector2 attackCenter = Utils::getCenterOfBounding(orbitalWeapon.attackBounding);

	int attackId = gameContext.entityMan->createAttack(gameContext, { orbitalWeapon.attackPosition.x-attackCenter.x, orbitalWeapon.attackPosition.y }, 0.f, attackGOtype);

	ColliderComponent& colliderComp = gameContext.entityMan->getComponent<ColliderComponent>(attackId);
	AttackComponent& attackComp = gameContext.entityMan->getComponent<AttackComponent>(attackId);
	SituationComponent& attackSit = gameContext.entityMan->getComponent<SituationComponent>(attackId);

	colliderComp.boundingRoot.bounding = orbitalWeapon.attackBounding;
	attackComp.damage = orbitalWeapon.damage;
	attackComp.maxLifetime = orbitalWeapon.attackLifetime;
	attackSit.noWorldDelete = true;


	orbitalWeapon.generateAttackTimeCounter = 0.f;
	orbitalWeapon.activated = false;

	// Play orbital attack sound
	gameContext.getSoundFacadeRef().loadSound(orbitalWeapon.attackSound.soundPath);
	gameContext.getSoundFacadeRef().playSound(orbitalWeapon.attackSound);
}



void OrbitalWeaponSystem::calculateBoundingToFloor(GameEngine& gameContext, SituationComponent& objectiveSituation, OrbitalWeaponComponent& orbitalWeapon) const { // changes the orbital Weapon attack bounding to the floor from the 0 in y
	SituationComponent* closestWall = Utils::getClosestWallXToObjetive(gameContext, objectiveSituation, true);

	orbitalWeapon.attackBounding.yUp = 0.f;

	if (closestWall != nullptr) {
		// Check if the situation of the objective is inside of the size x of the wall
		ColliderComponent& wallCollider = gameContext.entityMan->getComponent<ColliderComponent>(closestWall->id);

		BoundingBox wallWorldBounding = Utils::moveToWorldCoords(wallCollider.boundingRoot.bounding, *closestWall);

		if (objectiveSituation.position.x >= wallWorldBounding.xLeft && objectiveSituation.position.x <= wallWorldBounding.xRight) {
			// Data when there is floor
			orbitalWeapon.attackBounding.yDown = closestWall->position.y;

		}
		else {
			// Data when there is no floor
			orbitalWeapon.attackBounding.yDown = 100000.f;
		}
	}
	else {
		// Data when there is no floor
		orbitalWeapon.attackBounding.yDown =  100000.f;
	}
}
