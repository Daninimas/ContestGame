#include "StaticEntitiesSystem.hpp"

#include <eng/GameEngine.hpp>

#include <enum/EnumsIncluder>

#include <iostream>


StaticEntitiesSystem::StaticEntitiesSystem() {}

StaticEntitiesSystem::~StaticEntitiesSystem() {}

void StaticEntitiesSystem::update(GameEngine& gameContext) const {}


void StaticEntitiesSystem::init(GameEngine &gameContext) const {
    createPlayer(gameContext);

    gameContext.entityMan.createWall(gameContext, 0.f, 400.f, 0.f, GameObjectType::WALL);
    gameContext.entityMan.createWall(gameContext, 100.f, 380.f, 0.f, GameObjectType::WALL);

}

void StaticEntitiesSystem::createPlayer(GameEngine &gameContext) const {
    gameContext.entityMan.createPlayer(gameContext, 0.f, 0.f, 0.f, GameObjectType::PLAYER_GENERAL);
}