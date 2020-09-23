#include "StaticEntitiesSystem.hpp"

#include <eng/GameEngine.hpp>

#include <enum/EnumsIncluder>

#include <iostream>


StaticEntitiesSystem::StaticEntitiesSystem() {}

StaticEntitiesSystem::~StaticEntitiesSystem() {}

void StaticEntitiesSystem::update(GameEngine& gameContext) const {}


void StaticEntitiesSystem::init(GameEngine &gameContext) const {
    createPlayer(gameContext);

    gameContext.entityMan.createWall(gameContext, 0.f, 400.f, 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, 100.f, 390.f, 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, 201.f, 390.f, 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, 302.f, 390.f, 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, 403.f, 390.f, 0.f, GameObjectType::NONE);
    gameContext.entityMan.createWall(gameContext, 201.f, 320.f, 0.f, GameObjectType::NONE);


    gameContext.entityMan.createEnemy(gameContext, 400.f, 300.f, 0.f, GameObjectType::NONE);
}

void StaticEntitiesSystem::createPlayer(GameEngine &gameContext) const {
    gameContext.entityMan.createPlayer(gameContext, 0.f, 0.f, 0.f, GameObjectType::PLAYER_GENERAL);
}