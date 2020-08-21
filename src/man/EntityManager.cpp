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




int EntityManager::createPlayer(GameEngine& gameContext, float x, float y, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();
    //gameContext.playerId = entityId;


    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    DrawableComponent& drawableComp = createComponent<DrawableComponent>(entityId);

    //######### DATA ########//
    situation.x = x;
    situation.y = y;
    situation.rotation = r;

    drawableComp.sprite = "./TaOmA.png";

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);


    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(PLAYER, goType));
    return entityId;
}



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

