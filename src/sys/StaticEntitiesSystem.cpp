#include "StaticEntitiesSystem.hpp"

#include <eng/GameEngine.hpp>
#include <enum/EnumsIncluder>
#include <tools/MapLoader.hpp>
#include <iostream>


StaticEntitiesSystem::StaticEntitiesSystem() {}

StaticEntitiesSystem::~StaticEntitiesSystem() {}

void StaticEntitiesSystem::update(GameEngine& gameContext) const {}


void StaticEntitiesSystem::init(GameEngine &gameContext) const {
    createWorld(gameContext);

    createPlayer(gameContext);

    gameContext.entityMan.createWall(gameContext, Vector2( 0.f,   400.f ), 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, Vector2( 100.f, 400.f ), 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, Vector2( 200.f, 400.f ), 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, Vector2( 300.f, 400.f ), 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, Vector2( 400.f, 390.f ), 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, Vector2( 200.f, 320.f ), 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, Vector2( 600.f, 390.f ), 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, Vector2( 700.f, 390.f ), 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, Vector2( 800.f, 400.f ), 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, Vector2( 900.f, 410.f ), 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, Vector2( 1000.f, 410.f), 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, Vector2( 1100.f, 410.f), 0.f, GameObjectType::NONE);



    /*int chaserId = gameContext.entityMan.createEnemy(gameContext, 300.f, 300.f, 0.f, GameObjectType::CHASERJUMPER);
    gameContext.entityMan.getComponent<AIChaseComponent>(chaserId).objectiveId = WorldData::playerId;
    gameContext.entityMan.getComponent<AIMeleeAtkComponent>(chaserId).objectiveId = WorldData::playerId;*/

    int distanceEnemId = gameContext.entityMan.createEnemy(gameContext, Vector2(650.f, 300.f), 0.f, GameObjectType::DISTANCE_ENEMY);
    gameContext.entityMan.getComponent<AIDistanceAtkComponent>(distanceEnemId).objectiveId = WorldData::playerId;

    int transformEnemId = gameContext.entityMan.createEnemy(gameContext, Vector2(1100.f, 350.f), 0.f, GameObjectType::TRANSFORM_ENEMY);
    gameContext.entityMan.getComponent<AITransformationComponent>(transformEnemId).objectiveId = WorldData::playerId;

    /*int bomber = gameContext.entityMan.createEnemy(gameContext, 300.f, 70, 0.f, GameObjectType::BOMBER_ENEMY);
    gameContext.entityMan.getComponent<AIChaseComponent>(bomber).objectiveId = WorldData::playerId;*/

    /*int pouncerId = gameContext.entityMan.createEnemy(gameContext, 300.f, 300.f, 0.f, GameObjectType::POUNCER_ENEMY);
    gameContext.entityMan.getComponent<AIPounceComponent>(pouncerId).objectiveId = WorldData::playerId;
    gameContext.entityMan.getComponent<AIMeleeAtkComponent>(pouncerId).objectiveId = WorldData::playerId;*/

    int spawnId = gameContext.entityMan.createSpawner(gameContext, Vector2(300.f, 300.f), 0.f, GameObjectType::CHASERJUMPER);
    gameContext.entityMan.getComponent<SpawnerComponent>(spawnId).objectiveId = WorldData::playerId;

    gameContext.entityMan.createWeapon(gameContext, Vector2(250, 310.f), 0.f, GameObjectType::KNIFE);
    gameContext.entityMan.createWeapon(gameContext, Vector2(450, 360.f), 0.f, GameObjectType::M4);
    gameContext.entityMan.createWeapon(gameContext, Vector2(100.f, 200.f), 0.f, GameObjectType::GRENADE_LAUNCHER);
    gameContext.entityMan.createWeapon(gameContext, Vector2(200, 310.f), 0.f, GameObjectType::LASER_GUN);


    gameContext.entityMan.createCamera(gameContext, Vector2(0, 0), 0.f, GameObjectType::NONE);


    gameContext.entityMan.createPowerUp(gameContext, Vector2(150.f, 300), 0.f, GameObjectType::POWERUP_SHIELD);


    gameContext.entityMan.createHUDElement(gameContext, Vector2(10.f, 10.f), 0.f, GameObjectType::HUD_PLAYER_HEALTH);
    gameContext.entityMan.createHUDElement(gameContext, Vector2(10.f, 40.f), 0.f, GameObjectType::HUD_PLAYER_AMMO);

    MapLoader::loadMap(gameContext, "Media/Maps/prueba.json");
}

void StaticEntitiesSystem::createPlayer(GameEngine &gameContext) const {
    gameContext.entityMan.createPlayer(gameContext, Vector2(0.f, 200.f), 0.f, GameObjectType::PLAYER_GENERAL);
}

void StaticEntitiesSystem::createWorld(GameEngine& gameContext) const {
    gameContext.entityMan.createWorld(gameContext, WorldEnum::DEBUG);
}