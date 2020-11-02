#include "StaticEntitiesSystem.hpp"

#include <eng/GameEngine.hpp>

#include <enum/EnumsIncluder>

#include <iostream>


StaticEntitiesSystem::StaticEntitiesSystem() {}

StaticEntitiesSystem::~StaticEntitiesSystem() {}

void StaticEntitiesSystem::update(GameEngine& gameContext) const {}


void StaticEntitiesSystem::init(GameEngine &gameContext) const {
    createWorld(gameContext);

    createPlayer(gameContext);

    gameContext.entityMan.createWall(gameContext, 0.f,   380.f, 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, 100.f, 390.f, 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, 200.f, 400.f, 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, 300.f, 400.f, 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, 400.f, 390.f, 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, 200.f, 320.f, 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, 600.f, 390.f, 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, 700.f, 390.f, 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, 800.f, 400.f, 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, 900.f, 410.f, 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, 1000.f, 410.f, 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, 1100.f, 410.f, 0.f, GameObjectType::NONE);



    int chaserId = gameContext.entityMan.createEnemy(gameContext, 300.f, 300.f, 0.f, GameObjectType::CHASERJUMPER);
    gameContext.entityMan.getComponent<AIChaseComponent>(chaserId).objectiveId = WorldData::playerId;
    gameContext.entityMan.getComponent<AIMeleeAtkComponent>(chaserId).objectiveId = WorldData::playerId;

    int distanceEnemId = gameContext.entityMan.createEnemy(gameContext, 650.f, 300.f, 0.f, GameObjectType::DISTANCE_ENEMY);
    gameContext.entityMan.getComponent<AIDistanceAtkComponent>(distanceEnemId).objectiveId = WorldData::playerId;

    int transformEnemId = gameContext.entityMan.createEnemy(gameContext, 1100.f, 350.f, 0.f, GameObjectType::TRANSFORM_ENEMY);
    gameContext.entityMan.getComponent<AITransformationComponent>(transformEnemId).objectiveId = WorldData::playerId;

    int bomber = gameContext.entityMan.createEnemy(gameContext, 300.f, 70, 0.f, GameObjectType::BOMBER_ENEMY);
    gameContext.entityMan.getComponent<AIChaseComponent>(bomber).objectiveId = WorldData::playerId;

    gameContext.entityMan.createWeapon(gameContext, 250, 310.f, 0.f, GameObjectType::KNIFE);
    gameContext.entityMan.createWeapon(gameContext, 450, 360.f, 0.f, GameObjectType::M4);
    gameContext.entityMan.createWeapon(gameContext, 100.f, 200.f, 0.f, GameObjectType::GRENADE_LAUNCHER);
    gameContext.entityMan.createWeapon(gameContext, 200, 310.f, 0.f, GameObjectType::LASER_GUN);


    gameContext.entityMan.createCamera(gameContext, 0, 0, 0.f, GameObjectType::NONE);    
}

void StaticEntitiesSystem::createPlayer(GameEngine &gameContext) const {
    gameContext.entityMan.createPlayer(gameContext, 0.f, 200.f, 0.f, GameObjectType::PLAYER_GENERAL);
}

void StaticEntitiesSystem::createWorld(GameEngine& gameContext) const {
    gameContext.entityMan.createWorld(gameContext, WorldEnum::DEBUG);
}