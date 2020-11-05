#include "SpawnSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>

#include <iostream>


SpawnSystem::SpawnSystem() {}

SpawnSystem::~SpawnSystem() {}


void SpawnSystem::update(GameEngine& gameContext) const {
	auto& spawnerComponents = gameContext.entityMan.getComponents<SpawnerComponent>();

	for (SpawnerComponent& spawnComp : spawnerComponents) {
		spawnComp.cooldown += gameContext.getDeltaTime();

		if (spawnComp.cooldown > spawnComp.maxCooldown && spawnComp.maxSpawnedObjsAlive > spawnComp.spawnedObjsAlive.size() && gameContext.entityMan.existsComponent<SituationComponent>(spawnComp.objectiveId)) {
			manageSpawn(gameContext, spawnComp);
		}
	}
}


void SpawnSystem::manageSpawn(GameEngine& gameContext, SpawnerComponent& spawnComp) const {
	SituationComponent& spawnSit = gameContext.entityMan.getComponent<SituationComponent>(spawnComp.id);
	SituationComponent& objSit   = gameContext.entityMan.getComponent<SituationComponent>(spawnComp.objectiveId);

	if (Utils::objectiveInsideRange(spawnSit, objSit, spawnComp.rangeX, spawnComp.rangeY)) {
		spawnObject(gameContext, spawnComp, spawnSit);
	}
}

void SpawnSystem::spawnObject(GameEngine& gameContext, SpawnerComponent& spawnComp, SituationComponent& spawnSit) const {

	switch (spawnComp.spawnEntitiesType)
	{
	case EntityType::ENEMY:
		int newEntityID = gameContext.entityMan.createEnemy(gameContext, spawnSit.x, spawnSit.y, 0.f, spawnComp.spawnObjectsType);

		setobjectiveToAIComponents(gameContext, newEntityID, spawnComp.objectiveId);

		spawnComp.spawnedObjsAlive.push_back(newEntityID);
		break;
	}

	spawnComp.cooldown = 0.f;
	--spawnComp.numObjectsToSpawn;

	if (spawnComp.numObjectsToSpawn == 0) {
		// make the spawner die if it has ended spawning enemies
		gameContext.entityMan.getComponent<HealthComponent>(spawnComp.id).currentHealth = 0;
	}
}

void SpawnSystem::setobjectiveToAIComponents(GameEngine& gameContext, int newEntityID, int objectiveId) const {
	EntityManager& enttityMan = gameContext.entityMan;

	if (enttityMan.existsComponent<AIChaseComponent>(newEntityID)) {
		enttityMan.getComponent<AIChaseComponent>(newEntityID).objectiveId = objectiveId;
	}
	if (enttityMan.existsComponent<AIDistanceAtkComponent>(newEntityID)) {
		enttityMan.getComponent<AIDistanceAtkComponent>(newEntityID).objectiveId = objectiveId;
	}
	if (enttityMan.existsComponent<AIMeleeAtkComponent>(newEntityID)) {
		enttityMan.getComponent<AIMeleeAtkComponent>(newEntityID).objectiveId = objectiveId;
	}
	if (enttityMan.existsComponent<AIPounceComponent>(newEntityID)) {
		enttityMan.getComponent<AIPounceComponent>(newEntityID).objectiveId = objectiveId;
	}
	if (enttityMan.existsComponent<AITransformationComponent>(newEntityID)) {
		enttityMan.getComponent<AITransformationComponent>(newEntityID).objectiveId = objectiveId;
	}
}
