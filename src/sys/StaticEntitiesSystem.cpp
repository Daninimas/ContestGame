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

    gameContext.entityMan.createWall(gameContext, 0.f, 400.f, 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, 100.f, 390.f, 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, 200.f, 390.f, 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, 300.f, 390.f, 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, 400.f, 390.f, 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, 200.f, 320.f, 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, 600.f, 390.f, 0.f, GameObjectType::NONE);


    gameContext.entityMan.createEnemy(gameContext, 300.f, 300.f, 0.f, GameObjectType::NONE);
    gameContext.entityMan.createEnemy(gameContext, 650.f, 300.f, 0.f, GameObjectType::NONE);

    gameContext.entityMan.createWeapon(gameContext, 250, 310.f, 0.f, GameObjectType::KNIFE);
    gameContext.entityMan.createWeapon(gameContext, 450, 360.f, 0.f, GameObjectType::M4);


    gameContext.entityMan.createCamera(gameContext, 0, 0, 0.f, GameObjectType::NONE);


    // Prueba crear sonido
    //gameContext.getSoundFacadeRef().loadSound("Media/Sound/GE_KF7_Soviet.wav");
    //gameContext.getSoundFacadeRef().playSound("Media/Sound/GE_KF7_Soviet.wav");

    gameContext.getSoundFacadeRef().loadMusic("Media/Sound/delayscape_planet.ogg");
    gameContext.getSoundFacadeRef().playMusic("Media/Sound/delayscape_planet.ogg");
}

void StaticEntitiesSystem::createPlayer(GameEngine &gameContext) const {
    gameContext.entityMan.createPlayer(gameContext, 0.f, 300.f, 0.f, GameObjectType::PLAYER_GENERAL);
}

void StaticEntitiesSystem::createWorld(GameEngine& gameContext) const {
    WorldData::worldLimits = {0.f, 1000.f, 0.f, 500.f};
}