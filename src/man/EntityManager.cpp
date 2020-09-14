#include "EntityManager.hpp"

#include <tools/Utils.hpp>

#include <math.h>
#include <random>
#include <eng/GameEngine.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>

#include <iostream>


EntityManager::EntityManager() {
}

EntityManager::~EntityManager() {}


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

const std::vector<int> &EntityManager::getEntitiesToUpdate() {
    return entitiesToUpdate;
}

void EntityManager::addEntityToUpdate(const int id) { 
    if (std::find(entitiesToUpdate.begin(), entitiesToUpdate.end(), id) == entitiesToUpdate.end()) // if not found, insert
        entitiesToUpdate.emplace_back(id);
}

void EntityManager::clearEntitiesToUpdate() {
    entitiesToUpdate.clear();
}




// ------------------------------ GAME OBJECTS FACTORY

int EntityManager::createPlayer(GameEngine& gameContext, float x, float y, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();
    //gameContext.playerId = entityId;


    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    DrawableComponent& drawableComp = createComponent<DrawableComponent>(entityId);
    VelocityComponent& velocityComp = createComponent<VelocityComponent>(entityId);
    HeathComponent& heathComp = createComponent<HeathComponent>(entityId);
    MeleeWeaponComponent& meleWeaponComp = createComponent<MeleeWeaponComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);

    //######### DATA ########//
    situation.x = x;
    situation.y = y;
    situation.rotation = r;

    velocityComp.speedX = 30.f;

    drawableComp.sprite = "./TaOmA.png";

    gameContext.playerId = entityId;

    // Collider
    colliderComp.boundingRoot.bounding = { 0.f, 500.f, 0.f, 30.f };
    colliderComp.boundingRoot.childs.emplace_back( 20.f, 450.f, 10.f, 20.f ); //Head


    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::PLAYER, goType));
    return entityId;
}


int EntityManager::createAttack(GameEngine& gameContext, float x, float y, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();
    //gameContext.playerId = entityId;

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    createComponent<ColliderComponent>(entityId);
    createComponent<AttackComponent>(entityId);

    //######### DATA ########//
    situation.x = x;
    situation.y = y;
    situation.rotation = r;

    switch (goType)
    {
    case GameObjectType::MELEE_ATTACK:

        break;
    case GameObjectType::DISTANCE_ATTACK:
        createComponent<VelocityComponent>(entityId);
        break;
    }

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::ATTACK, goType));
    return entityId;
}


int EntityManager::createFloor(GameEngine& gameContext, float x, float y, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();
    //gameContext.playerId = entityId;

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    ColliderComponent& collider = createComponent<ColliderComponent>(entityId);

    //######### DATA ########//
    situation.x = x;
    situation.y = y;
    situation.rotation = r;

    collider.boundingRoot.bounding = { 0.f, 100.f, 0.f, 70.f };

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::ATTACK, goType));
    return entityId;
}