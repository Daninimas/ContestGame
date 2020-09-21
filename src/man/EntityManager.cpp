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
    // TODO Modifie this to not add new components all times
    eraseComponent<SituationComponent>(id);
    eraseComponent<RenderComponent>(id);
    eraseComponent<InputComponent>(id);
    eraseComponent<VelocityComponent>(id);
    eraseComponent<HeathComponent>(id);
    eraseComponent<ColliderComponent>(id);
    eraseComponent<MeleeWeaponComponent>(id);
    eraseComponent<WeaponComponent>(id);
    eraseComponent<AttackComponent>(id);
    eraseComponent<JumpComponent>(id);


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

void EntityManager::removeEntityToUpdate(const int id) {
    entitiesToUpdate.erase(std::remove(entitiesToUpdate.begin(), entitiesToUpdate.end(), id), entitiesToUpdate.end());
}

void EntityManager::clearEntitiesToUpdate() {
    entitiesToUpdate.clear();
}




// ------------------------------ GAME OBJECTS FACTORY

int EntityManager::createPlayer(GameEngine& gameContext, float x, float y, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();
    //gameContext.playerId = entityId;


    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    VelocityComponent& velocityComp = createComponent<VelocityComponent>(entityId);
    HeathComponent& heathComp = createComponent<HeathComponent>(entityId);
    MeleeWeaponComponent& meleWeaponComp = createComponent<MeleeWeaponComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    JumpComponent& jumpComp = createComponent<JumpComponent>(entityId);
    RenderComponent& renderComp = createComponent<RenderComponent>(entityId);

    //######### DATA ########//
    situation.x = x;
    situation.y = y;
    situation.rotation = r;

    velocityComp.speedX = 30.f;

    gameContext.playerId = entityId;

    // Collider
    colliderComp.collisionLayer = ColliderComponent::Player;
    colliderComp.type = ColliderType::DYNAMIC;
    //cout << (int)(0xFF & static_cast<uint8_t>(-ColliderComponent::PlayerAttack));
    //colliderComp.layerMasc = ColliderComponent::Wall | ColliderComponent::Enemy | ColliderComponent::Attack; //Collides with everything except PlayerAttacks
    colliderComp.layerMasc = 0xFF - ColliderComponent::PlayerAttack; //Collides with everything except PlayerAttacks
    colliderComp.boundingRoot.bounding = { 0.f, 500.f, 0.f, 30.f };
    colliderComp.boundingRoot.childs.emplace_back( 20.f, 450.f, 10.f, 20.f ); //Head

    // Jump
    jumpComp.jumptable = { 500.f, 500.f, 400.f, 400.f, 300.f, 300.f, 200.f, 100.f };

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
    ColliderComponent& collider = createComponent<ColliderComponent>(entityId);
    createComponent<VelocityComponent>(entityId);
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
        break;

    case GameObjectType::PLAYER_MELEE_ATTACK:
        collider.collisionLayer = ColliderComponent::PlayerAttack;
        collider.layerMasc = ColliderComponent::Enemy + ColliderComponent::Wall;  // Collides with enemys only
        collider.boundingRoot.bounding = { 0.f, 10.f, 0.f, 10.f };

        break;
    }

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::ATTACK, goType));
    return entityId;
}


int EntityManager::createWall(GameEngine& gameContext, float x, float y, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();
    //gameContext.playerId = entityId;

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    //RenderComponent& renderComp = createComponent<RenderComponent>(entityId);


    //######### DATA ########//
    situation.x = x;
    situation.y = y;
    situation.rotation = r;

    // Collider
    colliderComp.collisionLayer = ColliderComponent::Wall;
    colliderComp.layerMasc = ColliderComponent::Enemy + ColliderComponent::Player + ColliderComponent::PlayerAttack; //Collides with player and enemy
    colliderComp.boundingRoot.bounding = { 0.f, 100.f, 0.f, 70.f };

    //######### RENDER ########//
    //gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::ATTACK, goType));
    return entityId;
}