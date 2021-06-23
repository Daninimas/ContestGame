#include "TriggerSystem.hpp"

#include <eng/GameEngine.hpp>
#include <enum/TriggerFunction.hpp>
#include <tools/Utils.hpp>

#include <iostream>


TriggerSystem::TriggerSystem() {}

TriggerSystem::~TriggerSystem() {}


void TriggerSystem::update(GameEngine& gameContext) const {
	auto& triggers = gameContext.entityMan->getComponents<TriggerComponent>();
	std::vector<int> triggersToDelete;
	triggersToDelete.reserve(triggers.size());

	for (TriggerComponent& trigger : triggers) {
		ColliderComponent& collComp = gameContext.entityMan->getComponent<ColliderComponent>(trigger.id);

		if (Utils::checkCollidingWithEntity(collComp.boundingRoot, WorldElementsData::playerId)) {
			activateFunction(gameContext, trigger);
			triggersToDelete.emplace_back(trigger.id);
		}
	}


	for (int id : triggersToDelete) {
		gameContext.eraseEntityByID(id);
	}
}


void TriggerSystem::activateFunction(GameEngine& gameContext, TriggerComponent& trigger) const {

	for (TriggerFunction& function : trigger.functions) {

		switch (function)
		{
		case TriggerFunction::PLAY_MUSIC:
			playMusic(gameContext, trigger);
			break;

		case TriggerFunction::PLAY_SOUND:
			playSoundEffect(gameContext, trigger);
			break;

		case TriggerFunction::CREATE_ENTITY:
			createEntity(gameContext, trigger);
			break;

		case TriggerFunction::GIVE_CHILD_POINTS:
			WorldElementsData::playerScore += -(gameContext.entityMan->getComponent<HealthComponent>(trigger.id).score);
			break;

		case TriggerFunction::SPAWN_CITY_1:
			createSpawnsCity(gameContext);
			break;

		case TriggerFunction::SPAWN_BASE_1:
			createSpawnsBase(gameContext);
			break;

		case TriggerFunction::SPAWN_ORBITAL_LASER:
			createOrbitalStriker(gameContext, trigger);
			break;
		}
	}
}



// ---------------------- TRIGGER FUNCTIONS

void TriggerSystem::playMusic(GameEngine& gameContext, TriggerComponent& trigger) const {
	// Start music
	gameContext.getSoundFacadeRef().loadMusic(trigger.sound.soundPath);
	gameContext.getSoundFacadeRef().playMusic(trigger.sound);
}

void TriggerSystem::playSoundEffect(GameEngine& gameContext, TriggerComponent& trigger) const {
	// Start music
	gameContext.getSoundFacadeRef().loadSound(trigger.sound.soundPath);
	gameContext.getSoundFacadeRef().playSound(trigger.sound);
}

void TriggerSystem::createEntity(GameEngine& gameContext, TriggerComponent& trigger) const {
	// not functional yet
}

void TriggerSystem::createSpawnsBase(GameEngine& gameContext) const {
	int spawnBaseId = gameContext.entityMan->createSpawner(gameContext, Vector2(6698.f, -546.f), 0, GameObjectType::ENEMY_SPIDER);
	SpawnerComponent& spawnerBaseComp = gameContext.entityMan->getComponent<SpawnerComponent>(spawnBaseId);
	spawnerBaseComp.objectiveId = WorldElementsData::playerId;
	spawnerBaseComp.maxCooldown = 3.f;
	spawnerBaseComp.numObjectsToSpawn = 6;
	spawnerBaseComp.range = { 10000 , 10000 };

	int spawnBase2Id = gameContext.entityMan->createSpawner(gameContext, Vector2(5352.f, -482.f), 0, GameObjectType::DISTANCE_WALKING_ENEMY);
	SpawnerComponent& spawnerBase2Comp = gameContext.entityMan->getComponent<SpawnerComponent>(spawnBase2Id);
	spawnerBase2Comp.objectiveId = WorldElementsData::playerId;
	spawnerBase2Comp.maxCooldown = 5.f;
	spawnerBase2Comp.numObjectsToSpawn = 5;
	spawnerBase2Comp.range = { 10000 , 10000 };

	int spawnBase3Id = gameContext.entityMan->createSpawner(gameContext, Vector2(6788.f, -896.f), 0, GameObjectType::BOMBER_ENEMY);
	SpawnerComponent& spawnerBase3Comp = gameContext.entityMan->getComponent<SpawnerComponent>(spawnBase3Id);
	spawnerBase3Comp.objectiveId = WorldElementsData::playerId;
	spawnerBase3Comp.maxCooldown = 7.f;
	spawnerBase3Comp.numObjectsToSpawn = 3;
	spawnerBase3Comp.range = { 10000 , 10000 };

	int spawnBase4Id = gameContext.entityMan->createSpawner(gameContext, Vector2(5260.f, -866.f), 0, GameObjectType::FLYING_SHOOTER_ENEMY);
	SpawnerComponent& spawnerBase4Comp = gameContext.entityMan->getComponent<SpawnerComponent>(spawnBase4Id);
	spawnerBase4Comp.objectiveId = WorldElementsData::playerId;
	spawnerBase4Comp.maxCooldown = 4.5f;
	spawnerBase4Comp.numObjectsToSpawn = 2;
	spawnerBase4Comp.range = { 10000 , 10000 };
}


void TriggerSystem::createSpawnsCity(GameEngine& gameContext) const {
	int spawnId = gameContext.entityMan->createSpawner(gameContext, Vector2(7200.f, 520.f), 0, GameObjectType::CHASERJUMPER);
	SpawnerComponent& spawnerComp = gameContext.entityMan->getComponent<SpawnerComponent>(spawnId);
	spawnerComp.objectiveId = WorldElementsData::playerId;
	spawnerComp.maxCooldown = 2.5f;
	spawnerComp.numObjectsToSpawn = 7;
	spawnerComp.range = { 10000 , 1000 };

	int spawn2Id = gameContext.entityMan->createSpawner(gameContext, Vector2(6000.f, 520.f), 0, GameObjectType::POUNCER_ENEMY);
	SpawnerComponent& spawner2Comp = gameContext.entityMan->getComponent<SpawnerComponent>(spawn2Id);
	spawner2Comp.objectiveId = WorldElementsData::playerId;
	spawner2Comp.maxCooldown = 2.f;
	spawner2Comp.numObjectsToSpawn = 7;
	spawner2Comp.range = { 10000 , 1000 };
}


void TriggerSystem::createOrbitalStriker(GameEngine& gameContext, TriggerComponent& trigger) const {
	int orbitalStrikerID = gameContext.entityMan->createOrbitalStrikerEnemy(gameContext, gameContext.entityMan->getComponent<SituationComponent>(trigger.id).position, GameObjectType::ORBITAL_STRIKER);
	gameContext.entityMan->getComponent<AIOrbitalAtkComponent>(orbitalStrikerID).objectiveId = WorldElementsData::playerId;
}