#include "EntityManager.hpp"

#include <tools/Utils.hpp>

#include <math.h>
#include <random>
#include <eng/GameEngine.hpp>

//#include <tools/json.hpp>
//#include <json/value.h>
//#include <json/reader.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>

#include <iostream>
using namespace std;
//using nlohmann::json;



const bool SHOW_MELEEATTACKS     = false;

const bool DISABLED_VISIBLE_ZONE = false;

const bool ENABLED_AMBIENT_MUSIC = true;

EntityManager::EntityManager() {
}

EntityManager::~EntityManager() {}



/*
int EntityManager::createEntity (GameEngine &gameContext, EntityType eType, float x, float y, float z, GameObjectType goType) {
    int entityId = 0;
    switch(eType) {
    case PLAYER: {
        entityId = createPlayer(gameContext, x, y, z, goType);
        break;
    }
    case ENEMY: {
        entityId = createEnemy(gameContext, x, y, z, goType);
        break;
    }
    }//END SWITCH
    return entityId;
}
*/


Entity &EntityManager::getEntity(int id) {
    return entityMap.at(id);
}


void EntityManager::eraseEntityByID(int id) {
    // TODO Modifie this to not add news component all times
    eraseComponent<SituationComponent>              (id); //1

    entityMap           .erase(id);
}

std::unordered_map<int, Entity> &EntityManager::getEntities() {
    return entityMap;
}

std::vector<int> &EntityManager::getEntitiesToUpdate() {
    return entitiesToUpdate;
}

void EntityManager::addEntityToUpdate(const int id) {
    entitiesToUpdate.emplace_back(id);
}

void EntityManager::clearEntitiesToUpdate() {
    entitiesToUpdate.clear();
}

