#include "EntityManager.hpp"

#include <tools/Utils.hpp>
#include <tools/Sound.hpp>
#include <tools/WorldData.hpp>

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
    eraseComponent<DistanceWeaponComponent>(id);
    eraseComponent<AttackComponent>(id);
    eraseComponent<JumpComponent>(id);
    eraseComponent<SensorComponent>(id);
    eraseComponent<CameraComponent>(id);
    eraseComponent<MenuComponent>(id);
    eraseComponent<WeaponComponent>(id);
    eraseComponent<ExplosionAttackComponent>(id);
    eraseComponent<BombComponent>(id);

    // AI
    eraseComponent<AIChaseComponent>(id);
    eraseComponent<AIMeleeAtkComponent>(id);
    eraseComponent<AIDistanceAtkComponent>(id);

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




// ------------------------------ GAME OBJECTS FACTORY ------------------------------

int EntityManager::createPlayer(GameEngine& gameContext, float x, float y, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();


    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    VelocityComponent& velocityComp = createComponent<VelocityComponent>(entityId);
    HeathComponent& heathComp = createComponent<HeathComponent>(entityId);
    MeleeWeaponComponent& meleeWeaponComp = createComponent<MeleeWeaponComponent>(entityId);
    DistanceWeaponComponent& distanceWeaponComp = createComponent<DistanceWeaponComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    JumpComponent& jumpComp = createComponent<JumpComponent>(entityId);
    RenderComponent& renderComp = createComponent<RenderComponent>(entityId);
    SensorComponent& sensorComp = createComponent<SensorComponent>(entityId);
    InputComponent& inputComp = createComponent<InputComponent>(entityId);

    //######### DATA ########//
    situation.x = x;
    situation.y = y;
    situation.rotation = r;

    velocityComp.speedX = 100.f;
    velocityComp.gravity = 250.f;

    WorldData::playerId = entityId;

    // Collider
    colliderComp.collisionLayer = ColliderComponent::Player;
    colliderComp.type = ColliderType::DYNAMIC;
    colliderComp.layerMasc = 0xFF - ColliderComponent::PlayerAttack; //Collides with everything except PlayerAttacks
    colliderComp.boundingRoot.bounding = { 0.f, 50.f, 0.f, 50.f };
    colliderComp.boundingRoot.childs.emplace_back( 20.f, 30.f, 10.f, 20.f ); //Head

    // Melee
    meleeWeaponComp.attackBounding = { 0.f, 10.f, 0.f, 10.f };
    meleeWeaponComp.damage = 2;
    meleeWeaponComp.attackLifetime = 0.15f;

    // Distance
    distanceWeaponComp.attackBounding = { 0.f, 5.f, 0.f, 10.f };
    distanceWeaponComp.damage = 1;
    distanceWeaponComp.attackGeneralVelociy = 500.f;
    distanceWeaponComp.attackGravity = 100.f;
    distanceWeaponComp.maxCooldown = 0.5f;
    distanceWeaponComp.attackGeneratedType = DistanceWeaponComponent::BULLET;

    // Render component
    renderComp.sprite = "Media/Images/YellowDuck.png";
    renderComp.spriteRect = { 4, 34, 4, 34 };

    // Jump
    jumpComp.impulse = -200.f;
    //jumpComp.jumptable = { 500.f, 500.f, 400.f, 400.f, 300.f, 300.f, 200.f, 100.f };
    
    // Sensor
    sensorComp.sensorBounding = {25.f, 100.f, 0.f, 50.f};
    sensorComp.sensorLayerMasc = ColliderComponent::Enemy; // Sensors only enemies


    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::PLAYER, goType));
    return entityId;
}


int EntityManager::createAttack(GameEngine& gameContext, float x, float y, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    ColliderComponent& collider = createComponent<ColliderComponent>(entityId);
    AttackComponent& attack = createComponent<AttackComponent>(entityId);

    //######### DATA ########//
    situation.x = x;
    situation.y = y;
    situation.rotation = r;

    collider.type = ColliderType::NO_SOLID;

    switch (goType)
    {
    case GameObjectType::MELEE_ATTACK:
        collider.collisionLayer = ColliderComponent::Attack;
        collider.layerMasc = ColliderComponent::Player;  // Collides with player only

        attack.type = AttackType::MELEE;
        break;

    case GameObjectType::DISTANCE_ATTACK:
        createComponent<VelocityComponent>(entityId);
        collider.collisionLayer = ColliderComponent::Attack;
        collider.layerMasc = ColliderComponent::Player + ColliderComponent::Wall;  // Collides with player and walls

        attack.type = AttackType::DISTANCE;
        break;

    case GameObjectType::PLAYER_MELEE_ATTACK:
        collider.collisionLayer = ColliderComponent::PlayerAttack;
        collider.layerMasc = ColliderComponent::Enemy;  // Collides with enemies only

        attack.type = AttackType::MELEE;
        break;

    case GameObjectType::PLAYER_DISTANCE_ATTACK:
        createComponent<VelocityComponent>(entityId);

        collider.collisionLayer = ColliderComponent::PlayerAttack;
        collider.layerMasc = ColliderComponent::Enemy + ColliderComponent::Wall;  // Collides with enemies and walls

        attack.type = AttackType::DISTANCE;
        break;

    case GameObjectType::EXPLOSION:
        createComponent<ExplosionAttackComponent>(entityId);
        attack.type = AttackType::EXPLOSION;
        break;
    }

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::ATTACK, goType));
    return entityId;
}


int EntityManager::createWall(GameEngine& gameContext, float x, float y, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    //RenderComponent& renderComp = createComponent<RenderComponent>(entityId);


    //######### DATA ########//
    situation.x = x;
    situation.y = y;
    situation.rotation = r;

    // Collider
    colliderComp.collisionLayer = ColliderComponent::Wall;
    colliderComp.layerMasc = ColliderComponent::Enemy + ColliderComponent::Player + ColliderComponent::PlayerAttack + ColliderComponent::Weapon; //Collides with player and enemy
    colliderComp.boundingRoot.bounding = { 0.f, 100.f, 0.f, 10.f };

    //######### RENDER ########//
    //gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::WALL, goType));
    return entityId;
}


int EntityManager::createEnemy(GameEngine& gameContext, float x, float y, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();


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
    renderComp.sprite = "Media/Images/Elfo del Bosque.jpg";
    renderComp.spriteRect = { 1020, 1080, 1080, 1153 };

    // Velocity
    velocityComp.gravity = 230.f;

    // HeathComponent
    healthComp.maxHealth = 5;


    if (goType == GameObjectType::CHASER) {
        velocityComp.speedX = 70.f;
        renderComp.color = { 255, 100, 30, 255 };
        healthComp.maxHealth = 3;

        createComponent<AIChaseComponent>(entityId);
    }

    else if (goType == GameObjectType::CHASERJUMPER) {
        velocityComp.speedX = 50.f;
        renderComp.color = { 10, 20, 255, 255 };
        healthComp.maxHealth = 3;

        createComponent<AIChaseComponent>(entityId);
        createComponent<AIMeleeAtkComponent>(entityId);
        JumpComponent& jumpComp = createComponent<JumpComponent>(entityId);
        SensorComponent& sensorComp = createComponent<SensorComponent>(entityId);

        sensorComp.sensorLayerMasc = ColliderComponent::Player + ColliderComponent::Wall;
        sensorComp.sensorBounding = { 25.f, 75.f, 2.f, 48.f };

        jumpComp.impulse = -150.f;


        MeleeWeaponComponent& meleeWeaponComp = createComponent<MeleeWeaponComponent>(entityId);
        meleeWeaponComp.attackBounding = { 0.f, 20.f, 10.f, 40.f };
        meleeWeaponComp.damage = 2;
        meleeWeaponComp.maxCooldown = 1.5f;
    }
    
    else if(goType == GameObjectType::DISTANCE_ENEMY) {
        velocityComp.speedX = 0.f;
        renderComp.color = { 10, 255, 20, 255 };
        healthComp.maxHealth = 7;

        AIDistanceAtkComponent& distanceAIComp = createComponent<AIDistanceAtkComponent>(entityId);
        distanceAIComp.rangeX = 200.f;
        distanceAIComp.rangeY = 40.f;

        DistanceWeaponComponent& distanceWeaponComp = createComponent<DistanceWeaponComponent>(entityId);
        // Distance
        distanceWeaponComp.attackBounding = { 0.f, 5.f, 0.f, 10.f };
        distanceWeaponComp.damage = 1;
        distanceWeaponComp.attackGeneralVelociy = 300.f;
        distanceWeaponComp.attackGravity = 100.f;
        distanceWeaponComp.maxCooldown = 1.f;
    }

    healthComp.resetHealth(); // Reset health to set the current health the same as maxHealth

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::ENEMY, goType));
    return entityId;
}


int EntityManager::createWeapon(GameEngine& gameContext, float x, float y, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();


    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    RenderComponent& renderComp = createComponent<RenderComponent>(entityId);
    VelocityComponent& velocityComp = createComponent<VelocityComponent>(entityId); // To add gravity


    //######### DATA ########//
    situation.x = x;
    situation.y = y;
    situation.rotation = r;

    // Collider
    colliderComp.collisionLayer = ColliderComponent::Weapon;
    colliderComp.layerMasc = ColliderComponent::Player + ColliderComponent::Wall; //Collides with player and wall
    colliderComp.boundingRoot.bounding = { 0.f, 10.f, 0.f, 10.f };
    colliderComp.type = ColliderType::DYNAMIC;

    // Render component
    renderComp.sprite = "Media/Images/TaOmA.png";
    renderComp.spriteRect = { 400, 450, 200, 250 };

    // Velocity
    velocityComp.gravity = 80.f;
    velocityComp.speedX  = 0.f;



    if (goType == GameObjectType::M4) {
        DistanceWeaponComponent& distanceWeaponComp = createComponent<DistanceWeaponComponent>(entityId);

        distanceWeaponComp.attackBounding = { 0.f, 5.f, 0.f, 10.f };
        distanceWeaponComp.damage = 1;
        distanceWeaponComp.attackGeneralVelociy = 900.f;
        distanceWeaponComp.attackGravity = 0.f;
        distanceWeaponComp.maxCooldown = 0.2f;
        distanceWeaponComp.attackLifetime = 1.5f;
        distanceWeaponComp.attackGeneratedType = DistanceWeaponComponent::BULLET;

        distanceWeaponComp.attackSound.soundPath = "Media/Sound/GE_KF7_Soviet.wav";

        WorldData::worldDistanceWeapons.push_back(entityId);
    }

    else if (goType == GameObjectType::KNIFE) {
        MeleeWeaponComponent& meleeWeaponComp = createComponent<MeleeWeaponComponent>(entityId);

        meleeWeaponComp.attackBounding = { 0.f, 20.f, 10.f, 40.f };
        meleeWeaponComp.damage = 4;
        meleeWeaponComp.maxCooldown = 1.f;
        meleeWeaponComp.attackLifetime = 0.1f;

        meleeWeaponComp.attackSound.soundPath = "Media/Sound/GE_KF7_Soviet.wav";


        WorldData::worldMeleeWeapons.push_back(entityId);
    }

    else if (goType == GameObjectType::GRENADE_LAUNCHER) {
        DistanceWeaponComponent& distanceWeaponComp = createComponent<DistanceWeaponComponent>(entityId);

        distanceWeaponComp.attackBounding = { 0.f, 20.f, 0.f, 20.f };
        distanceWeaponComp.damage = 10;
        distanceWeaponComp.attackGeneralVelociy = 100.f;
        distanceWeaponComp.attackGravity = 100.f;
        distanceWeaponComp.maxCooldown = 0.5f;
        distanceWeaponComp.attackLifetime = 0.5f;
        distanceWeaponComp.attackGeneratedType = DistanceWeaponComponent::BOMB;

        distanceWeaponComp.attackSound.soundPath = "Media/Sound/GE_KF7_Soviet.wav";

        WorldData::worldDistanceWeapons.push_back(entityId);
    }

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::WEAPON, goType));
    return entityId;
}


int EntityManager::createCamera(GameEngine& gameContext, float x, float y, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();

    WorldData::activeCameraId = entityId;

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    CameraComponent& cameraComp   = createComponent<CameraComponent>(entityId);
    VelocityComponent& velocityComp = createComponent<VelocityComponent>(entityId);


    //######### DATA ########//
    situation.x = x;
    situation.y = y;
    situation.rotation = r;

    // Camera
    cameraComp.viewRect = { 0.f, 600, 0, 400 };
    cameraComp.zoom = 1.f;
    cameraComp.offsetX = 50.f;
    cameraComp.offsetY = 50.f;


    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createCamera(gameContext, entityId);

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::CAMERA, goType));
    return entityId;
}


int EntityManager::createBomb(GameEngine& gameContext, float x, float y, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    BombComponent& bombComp = createComponent<BombComponent>(entityId);
    createComponent<VelocityComponent>(entityId);

    //######### DATA ########//
    situation.x = x;
    situation.y = y;
    situation.rotation = r;

    colliderComp.type = ColliderType::DYNAMIC;

    switch (goType) {
        case GameObjectType::BOMB:
        colliderComp.collisionLayer = ColliderComponent::Attack;
        colliderComp.layerMasc = ColliderComponent::Player + ColliderComponent::Wall;
            break;

        case GameObjectType::PLAYER_BOMB:
            colliderComp.collisionLayer = ColliderComponent::PlayerAttack;
            colliderComp.layerMasc = ColliderComponent::Enemy + ColliderComponent::Wall;
            break;
    }
    

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::BOMB, goType));
    return entityId;
}


// ------------------------------ WORLD CREATION ------------------------------

void EntityManager::createWorld(GameEngine& gameContext, WorldEnum worldName) {

    switch (worldName)
    {
    case WorldEnum::DEBUG:
        // World limits
        WorldData::worldLimits = { 0.f, 1000.f, 0.f, 500.f };

        // World music
        WorldData::worldMusic.soundPath = "Media/Sound/delayscape_planet.ogg";
        WorldData::worldMusic.pitch = 2.f;
        WorldData::worldMusic.volume = 100.f;

        gameContext.getSoundFacadeRef().loadMusic(WorldData::worldMusic.soundPath);
        gameContext.getSoundFacadeRef().playMusic(WorldData::worldMusic);

        break;
    }
}


// ------------------------------ MENUS CREATION ------------------------------

int EntityManager::createMenu(GameEngine& gameContext, GameObjectType menuType) {
    int entityId = Entity::getCurrentId();

    MenuComponent& menuComp = createComponent<MenuComponent>(entityId);

    switch (menuType)
    {
    case GameObjectType::PAUSE:
        menuComp.options.emplace_back(MenuOptions::PLAY);
        menuComp.options.emplace_back(MenuOptions::EXIT);

        break;
    }

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::MENU, menuType));
    return entityId;
}
