#include "StaticEntitiesSystem.hpp"

#include <eng/GameEngine.hpp>

#include <enum/EnumsIncluder>

#include <iostream>


StaticEntitiesSystem::StaticEntitiesSystem() {}

StaticEntitiesSystem::~StaticEntitiesSystem() {}

void StaticEntitiesSystem::init(GameEngine &gameContext) const {
    createPlayer(gameContext);
}

void StaticEntitiesSystem::createPlayer(GameEngine &gameContext) const {
    gameContext.entityMan.createPlayer(gameContext, 0.f, 0.f, 0.f, PLAYER_GENERAL);
}