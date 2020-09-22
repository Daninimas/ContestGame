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
    MeleeWeaponComponent& meleeWeaponComp = createComponent<MeleeWeaponComponent>(entityId);
    DistanceWeaponComponent& distanceWeaponComp = createComponent<DistanceWeaponComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    JumpComponent& jumpComp = createComponent<JumpComponent>(entityId);
    RenderComponent& renderComp = createComponent<RenderComponent>(entityId);

    //######### DATA ########//
    situation.x = x;
    situation.y = y;
    situation.rotation = r;

    velocityComp.speedX = 30.f;
    velocityComp.gravity = 50.f;

    gameContext.playerId = entityId;

    // Collider
    colliderComp.collisionLayer = ColliderComponent::Player;
    colliderComp.type = ColliderType::DYNAMIC;
    colliderComp.layerMasc = 0xFF - ColliderComponent::PlayerAttack; //Collides with everything except PlayerAttacks
    colliderComp.boundingRoot.bounding = { 0.f, 50.f, 0.f, 50.f };
    colliderComp.boundingRoot.childs.emplace_back( 20.f, 30.f, 10.f, 20.f ); //Head

    // Melee
    meleeWeaponComp.attackBounding = { 0.f, 10.f, 0.f, 10.f };
    meleeWeaponComp.damage = 2;

    // Distance
    distanceWeaponComp.attackBounding = { 0.f, 5.f, 0.f, 10.f };
    distanceWeaponComp.damage = 1;
    distanceWeaponComp.attackGeneralVelociy = 500.f;
    distanceWeaponComp.attackGravity = 0.01f;
    distanceWeaponComp.maxCooldown = 0.5f;

    // Render component
    renderComp.sprite = "TaOmA.png";
    renderComp.spriteRect = { 247, 350, 127, 165 };

    // Jump
    //jumpComp.jumptable = { 500.f, 500.f, 400.f, 400.f, 300.f, 300.f, 200.f, 100.f };

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
    AttackComponent& attack = createComponent<AttackComponent>(entityId);

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

    case GameObjectType::PLAYER_MELEE_ATTACK:
        collider.collisionLayer = ColliderComponent::PlayerAttack;
        collider.layerMasc = ColliderComponent::Enemy;  // Collides with enemys only

        attack.type = AttackType::MELEE;

        break;

    case GameObjectType::PLAYER_DISTANCE_ATTACK:
        createComponent<VelocityComponent>(entityId);

        collider.collisionLayer = ColliderComponent::PlayerAttack;
        collider.layerMasc = ColliderComponent::Enemy + ColliderComponent::Wall;  // Collides with enemys and walls

        attack.type = AttackType::DISTANCE;
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
    colliderComp.boundingRoot.bounding = { 0.f, 100.f, 0.f, 10.f };

    //######### RENDER ########//
    //gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::WALL, goType));
    return entityId;
}


int EntityManager::createEnemy(GameEngine& gameContext, float x, float y, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();
    //gameContext.playerId = entityId;

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    RenderComponent& renderComp = createComponent<RenderComponent>(entityId);
    VelocityComponent& velocityComp = createComponent<VelocityComponent>(entityId);
    HeathComponent& healthComp = createComponent<HeathComponent>(entityId);


    //######### DATA ########//
    situation.x = x;
    situation.y = y;
    situation.rotation = r;

    // Collider
    colliderComp.collisionLayer = ColliderComponent::Enemy;
    colliderComp.layerMasc = ColliderComponent::Player + ColliderComponent::PlayerAttack + ColliderComponent::Wall; //Collides with player and enemy
    colliderComp.boundingRoot.bounding = { 0.f, 50.f, 0.f, 50.f };
    colliderComp.type = ColliderType::DYNAMIC;

    // Render component
    renderComp.sprite = "TaOmA.png";
    renderComp.spriteRect = { 400, 450, 200, 250 };

    // Velocity
    velocityComp.gravity = 20.f;

    // HeathComponent
    healthComp.maxHealth = 5;


    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::ENEMY, goType));
    return entityId;
}