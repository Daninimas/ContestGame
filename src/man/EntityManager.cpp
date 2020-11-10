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
    eraseComponent<HealthComponent>(id);
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
    eraseComponent<DodgeComponent>(id);
    eraseComponent<SpawnerComponent>(id);
    eraseComponent<ShieldComponent>(id);
    eraseComponent<PowerUpComponent>(id);

    // AI
    eraseComponent<AIChaseComponent>(id);
    eraseComponent<AIMeleeAtkComponent>(id);
    eraseComponent<AIDistanceAtkComponent>(id);
    eraseComponent<AITransformationComponent>(id);
    eraseComponent<AIDropBombComponent>(id);
    eraseComponent<AIPounceComponent>(id);

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

int EntityManager::createPlayer(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();


    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    VelocityComponent& velocityComp = createComponent<VelocityComponent>(entityId);
    HealthComponent& heathComp = createComponent<HealthComponent>(entityId);
    MeleeWeaponComponent& meleeWeaponComp = createComponent<MeleeWeaponComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    JumpComponent& jumpComp = createComponent<JumpComponent>(entityId);
    RenderComponent& renderComp = createComponent<RenderComponent>(entityId);
    SensorComponent& sensorComp = createComponent<SensorComponent>(entityId);
    InputComponent& inputComp = createComponent<InputComponent>(entityId);
    DodgeComponent& dodgeComp = createComponent<DodgeComponent>(entityId);
    // Create the distance weapon
    Utils::setNormalPistolToEntity(gameContext, entityId);

    //######### DATA ########//
    situation.position = position;
    situation.rotation = r;
    situation.noWorldDelete = true;

    velocityComp.speedX = 100.f;
    velocityComp.gravity = 250.f;

    WorldData::playerId = entityId;

    // Collider
    colliderComp.collisionLayer = ColliderComponent::Player;
    colliderComp.type = ColliderType::DYNAMIC;
    colliderComp.layerMasc = 0xFF - ColliderComponent::PlayerAttack - ColliderComponent::PlayerShield; //Collides with everything except PlayerAttacks
    colliderComp.boundingRoot.bounding = { 0.f, 50.f, 0.f, 50.f };
    colliderComp.boundingRoot.childs.emplace_back( 20.f, 30.f, 10.f, 20.f ); //Head

    // Melee
    meleeWeaponComp.attackBounding = { 0.f, 10.f, 0.f, 10.f };
    meleeWeaponComp.damage = 2;
    meleeWeaponComp.attackLifetime = 0.15f;


    // Render component
    renderComp.sprite = "Media/Images/YellowDuck.png";
    renderComp.spriteRect = { 4, 34, 4, 34 };

    // Jump
    jumpComp.impulse = -200.f;
    //jumpComp.jumptable = { 500.f, 500.f, 400.f, 400.f, 300.f, 300.f, 200.f, 100.f };
    
    // Sensor
    sensorComp.sensorBounding = {25.f, 100.f, 2.f, 48.f};
    sensorComp.sensorLayerMasc = ColliderComponent::Enemy; // Sensors only enemies

    // Dodge
    dodgeComp.dodgeMaxDuration = 0.2f;
    dodgeComp.dodgeTime = 0.2f;
    dodgeComp.maxCooldown = 1.f;
    dodgeComp.velocityIncrementFactor = 4.f;
    dodgeComp.initDodgeComponent();


    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::PLAYER, goType));
    return entityId;
}


int EntityManager::createAttack(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    ColliderComponent& collider = createComponent<ColliderComponent>(entityId);
    AttackComponent& attack = createComponent<AttackComponent>(entityId);

    //######### DATA ########//
    situation.position = position;
    situation.rotation = r;

    collider.type = ColliderType::NO_SOLID;

    switch (goType)
    {
    case GameObjectType::MELEE_ATTACK:
        collider.collisionLayer = ColliderComponent::Attack;
        collider.layerMasc = ColliderComponent::Player + ColliderComponent::PlayerShield;  // Collides with player only

        attack.type = AttackType::MELEE;
        break;

    case GameObjectType::DISTANCE_ATTACK:
        createComponent<VelocityComponent>(entityId);
        collider.collisionLayer = ColliderComponent::Attack;
        collider.layerMasc = ColliderComponent::Player + ColliderComponent::Wall + ColliderComponent::PlayerShield;  // Collides with player and walls

        attack.type = AttackType::DISTANCE;
        break;

    case GameObjectType::PLAYER_MELEE_ATTACK:
        collider.collisionLayer = ColliderComponent::PlayerAttack;
        collider.layerMasc = ColliderComponent::Enemy + ColliderComponent::Shield;  // Collides with enemies only

        attack.type = AttackType::MELEE;
        break;

    case GameObjectType::PLAYER_DISTANCE_ATTACK:
        createComponent<VelocityComponent>(entityId);

        collider.collisionLayer = ColliderComponent::PlayerAttack;
        collider.layerMasc = ColliderComponent::Enemy + ColliderComponent::Wall + ColliderComponent::Shield;  // Collides with enemies and walls

        attack.type = AttackType::DISTANCE;
        break;

    case GameObjectType::EXPLOSION:
        createComponent<ExplosionAttackComponent>(entityId);
        attack.type = AttackType::EXPLOSION;
        break;

    case GameObjectType::PLAYER_LASER:
        createComponent<VelocityComponent>(entityId);

        collider.collisionLayer = ColliderComponent::PlayerAttack;
        collider.layerMasc = ColliderComponent::Enemy + ColliderComponent::Wall + ColliderComponent::Shield;  // Collides with enemies and walls

        attack.type = AttackType::LASER;
        break;

    case GameObjectType::LASER:
        createComponent<VelocityComponent>(entityId);
        collider.collisionLayer = ColliderComponent::Attack;
        collider.layerMasc = ColliderComponent::Player + ColliderComponent::Wall + ColliderComponent::PlayerShield;  // Collides with player and walls

        attack.type = AttackType::LASER;
        break;
    }

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::ATTACK, goType));
    return entityId;
}


int EntityManager::createWall(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    //RenderComponent& renderComp = createComponent<RenderComponent>(entityId);


    //######### DATA ########//
    situation.position = position;
    situation.rotation = r;

    // Collider
    colliderComp.collisionLayer = ColliderComponent::Wall;
    colliderComp.layerMasc = ColliderComponent::Enemy + ColliderComponent::Player + ColliderComponent::PlayerAttack + ColliderComponent::Weapon + ColliderComponent::Attack; //Collides with player and enemy
    colliderComp.boundingRoot.bounding = { 0.f, 100.f, 0.f, 10.f };

    //######### RENDER ########//
    //gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::WALL, goType));
    return entityId;
}


int EntityManager::createEnemy(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();


    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    RenderComponent& renderComp = createComponent<RenderComponent>(entityId);
    VelocityComponent& velocityComp = createComponent<VelocityComponent>(entityId);
    HealthComponent& healthComp = createComponent<HealthComponent>(entityId);


    //######### DATA ########//
    situation.position = position;
    situation.rotation = r;

    // Collider
    colliderComp.collisionLayer = ColliderComponent::Enemy;
    colliderComp.layerMasc = ColliderComponent::Player + ColliderComponent::PlayerAttack + ColliderComponent::Wall + ColliderComponent::PlayerShield; //Collides with player and enemy
    colliderComp.boundingRoot.bounding = { 0.f, 50.f, 0.f, 50.f };
    colliderComp.type = ColliderType::DYNAMIC;

    // Render component
    renderComp.sprite = "Media/Images/Elfo del Bosque.jpg";
    renderComp.spriteRect = { 1020, 1080, 1080, 1153 };

    // Velocity
    velocityComp.gravity = 230.f;

    // HealthComponent
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
        distanceAIComp.range.x = 200.f;
        distanceAIComp.range.y = 40.f;

        DistanceWeaponComponent& distanceWeaponComp = createComponent<DistanceWeaponComponent>(entityId);
        // Distance
        distanceWeaponComp.attackBounding = { 0.f, 5.f, 0.f, 10.f };
        distanceWeaponComp.damage = 1;
        distanceWeaponComp.attackGeneralVelociy = 300.f;
        distanceWeaponComp.attackGravity = 100.f;
        distanceWeaponComp.maxCooldown = 1.f;
    }

    else if (goType == GameObjectType::TRANSFORM_ENEMY) {
        velocityComp.speedX = 100.f;
        renderComp.color = { 255, 255, 255, 255 };
        healthComp.maxHealth = 5;

        JumpComponent& jumpComp = createComponent<JumpComponent>(entityId);
        SensorComponent& sensorComp = createComponent<SensorComponent>(entityId);

        sensorComp.sensorLayerMasc = ColliderComponent::Player + ColliderComponent::Wall;
        sensorComp.sensorBounding = { 25.f, 75.f, 2.f, 48.f };

        jumpComp.impulse = -150.f;

        MeleeWeaponComponent& meleeWeaponComp = createComponent<MeleeWeaponComponent>(entityId);
        meleeWeaponComp.attackBounding = { 0.f, 20.f, 10.f, 40.f };
        meleeWeaponComp.damage = 2;
        meleeWeaponComp.maxCooldown = 0.5f;

        AITransformationComponent& transformComp = createComponent<AITransformationComponent>(entityId);
        transformComp.newBoundingRoot.bounding = { 0.f, 100.f, 0.f, 100.f };
        transformComp.newColor = { 255, 10, 10, 255 };
        transformComp.newScale = { 2.f, 2.f };
        transformComp.newSprite = "Media/Images/Elfo del Bosque.jpg";
        transformComp.newSpriteRect = { 1020, 1080, 1080, 1153 };
        transformComp.range = { 150.f, 150.f };
    }

    else if (goType == GameObjectType::BOMBER_ENEMY) {
        velocityComp.speedX = 70.f;
        velocityComp.gravity = 0.f;
        renderComp.color = { 255, 100, 30, 255 };
        healthComp.maxHealth = 10;

        createComponent<AIChaseComponent>(entityId);

        DistanceWeaponComponent& distanceWeaponComp = createComponent<DistanceWeaponComponent>(entityId);

        distanceWeaponComp.attackBounding = { 0.f, 20.f, 0.f, 20.f };
        distanceWeaponComp.damage = 3;
        distanceWeaponComp.attackGeneralVelociy = 0.f;
        distanceWeaponComp.attackGravity = 100.f;
        distanceWeaponComp.maxCooldown = 0.5f;
        distanceWeaponComp.attackLifetime = 0.3f;
        distanceWeaponComp.attackGeneratedType = DistanceWeaponComponent::BOMB;

        distanceWeaponComp.attackSound.soundPath = "Media/Sound/Weapons/silbidoBombaCayendo.wav";

        distanceWeaponComp.explosionExpansion = 1.3f;
        distanceWeaponComp.explosionTime = 0.5f;
        distanceWeaponComp.startActivated = false;

        AIDropBombComponent& dropBombComp = createComponent<AIDropBombComponent>(entityId);
        dropBombComp.maxCooldown = 2.f;

    }

    else if (goType == GameObjectType::POUNCER_ENEMY) {
    velocityComp.speedX = 45.f;
    renderComp.color = { 20, 20, 30, 255 };
    healthComp.maxHealth = 4;


    createComponent<AIMeleeAtkComponent>(entityId);
    JumpComponent& jumpComp = createComponent<JumpComponent>(entityId);
    SensorComponent& sensorComp = createComponent<SensorComponent>(entityId);
    AIPounceComponent& pounceComp = createComponent<AIPounceComponent>(entityId);

    sensorComp.sensorLayerMasc = ColliderComponent::Player + ColliderComponent::Wall;
    sensorComp.sensorBounding = { 25.f, 60.f, 2.f, 48.f };

    jumpComp.impulse = -100.f;
    jumpComp.maxCooldown = 1.f;


    MeleeWeaponComponent& meleeWeaponComp = createComponent<MeleeWeaponComponent>(entityId);
    meleeWeaponComp.attackBounding = { 0.f, 20.f, 10.f, 40.f };
    meleeWeaponComp.damage = 2;
    meleeWeaponComp.maxCooldown = 1.5f;


    pounceComp.range.x = 200.f;
    pounceComp.range.y = 30.f;
    pounceComp.velocityIncFactor = 4.7f;
    pounceComp.maxCooldown = 2.f;
    }

    healthComp.resetHealth(); // Reset health to set the current health the same as maxHealth

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::ENEMY, goType));
    return entityId;
}


int EntityManager::createWeapon(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();


    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    RenderComponent& renderComp = createComponent<RenderComponent>(entityId);
    VelocityComponent& velocityComp = createComponent<VelocityComponent>(entityId); // To add gravity


    //######### DATA ########//
    situation.position = position;
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
        distanceWeaponComp.ammo = 100;
        distanceWeaponComp.infiniteAmmo = false;

        distanceWeaponComp.attackSound.soundPath = "Media/Sound/Weapons/M4A1_Single-Kibblesbob-8540445.wav";

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
        distanceWeaponComp.ammo = 10;
        distanceWeaponComp.infiniteAmmo = false;

        distanceWeaponComp.attackSound.soundPath = "Media/Sound/Weapons/grenadeLauncherSound.wav";

        distanceWeaponComp.explosionExpansion = 2.f;
        distanceWeaponComp.explosionTime = 0.2f;
        distanceWeaponComp.startActivated = false;

        WorldData::worldDistanceWeapons.push_back(entityId);
    }
    
    else if (goType == GameObjectType::LASER_GUN) {
        DistanceWeaponComponent& distanceWeaponComp = createComponent<DistanceWeaponComponent>(entityId);

        distanceWeaponComp.attackBounding = { 0.f, 5000.f, 0.f, 30.f };
        distanceWeaponComp.damage = 1;
        distanceWeaponComp.attackGeneralVelociy = 0.f;
        distanceWeaponComp.attackGravity = 0.f;
        distanceWeaponComp.maxCooldown = 0.6f;
        distanceWeaponComp.attackLifetime = 0.6f;
        distanceWeaponComp.attackGeneratedType = DistanceWeaponComponent::LASER;
        distanceWeaponComp.ammo = 30;
        distanceWeaponComp.infiniteAmmo = false;

        distanceWeaponComp.attackSound.soundPath = "Media/Sound/GE_KF7_Soviet.wav";

        WorldData::worldDistanceWeapons.push_back(entityId);
    }

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::WEAPON, goType));
    return entityId;
}


int EntityManager::createCamera(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();

    WorldData::activeCameraId = entityId;

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    CameraComponent& cameraComp   = createComponent<CameraComponent>(entityId);
    VelocityComponent& velocityComp = createComponent<VelocityComponent>(entityId);


    //######### DATA ########//
    situation.position = position;
    situation.rotation = r;
    situation.noWorldDelete = true;

    // Camera
    cameraComp.viewRect = { 0.f, 600, 0, 400 };
    cameraComp.zoom = 1.f;
    cameraComp.offset = { 50.f, 50.f};


    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createCamera(gameContext, entityId);

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::CAMERA, goType));
    return entityId;
}


int EntityManager::createBomb(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    BombComponent& bombComp = createComponent<BombComponent>(entityId);
    createComponent<VelocityComponent>(entityId);

    //######### DATA ########//
    situation.position = position;
    situation.rotation = r;

    colliderComp.type = ColliderType::DYNAMIC;

    switch (goType) {
        case GameObjectType::BOMB:
            colliderComp.collisionLayer = ColliderComponent::Attack;
            colliderComp.layerMasc = ColliderComponent::Player + ColliderComponent::Wall + ColliderComponent::PlayerShield;
            bombComp.explosionSound.soundPath = "Media/Sound/Weapons/alienExplosion.wav";
            break;

        case GameObjectType::PLAYER_BOMB:
            colliderComp.collisionLayer = ColliderComponent::PlayerAttack;
            colliderComp.layerMasc = ColliderComponent::Enemy + ColliderComponent::Wall + ColliderComponent::Shield;
            bombComp.explosionSound.soundPath = "Media/Sound/Weapons/explosion.wav";
            break;
    }
    

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::BOMB, goType));
    return entityId;
}


int EntityManager::createSpawner(GameEngine& gameContext, Vector2 position, float r, GameObjectType goTypeToSpawn) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    RenderComponent& renderComp = createComponent<RenderComponent>(entityId);
    HealthComponent& healthComp = createComponent<HealthComponent>(entityId);
    SpawnerComponent& spawnComp = createComponent<SpawnerComponent>(entityId);


    //######### DATA ########//
    situation.position = position;
    situation.rotation = r;
    situation.scale = { 0.1f, 0.1f };

    // Render component
    renderComp.sprite = "Media/Images/spawner.png";
    renderComp.spriteRect = { 0, 200, 0, 200 };

    // Collider
    colliderComp.collisionLayer = ColliderComponent::Enemy; // temporal
    colliderComp.layerMasc = ColliderComponent::Player + ColliderComponent::PlayerAttack + ColliderComponent::Wall + ColliderComponent::PlayerShield; //Collides with player and enemy
    colliderComp.boundingRoot.bounding = { 0.f, 20.f, 0.f, 20.f };
    colliderComp.type = ColliderType::STATIC;
    

    switch (goTypeToSpawn) {
    case GameObjectType::CHASERJUMPER:

        // Spawner
        spawnComp.maxCooldown = 5.f;
        spawnComp.range = { 200.f, 200.f };
        spawnComp.spawnObjectsType = GameObjectType::CHASERJUMPER;
        spawnComp.spawnEntitiesType = EntityType::ENEMY;

        // Health
        healthComp.maxHealth = 10;

        break;        
    }

    // Init health
    healthComp.resetHealth();

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::SPAWNER, goTypeToSpawn));
    return entityId;
}


int EntityManager::createShield(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    //RenderComponent& renderComp = createComponent<RenderComponent>(entityId);
    HealthComponent& healthComp = createComponent<HealthComponent>(entityId);
    MeleeWeaponComponent& meleeComp = createComponent<MeleeWeaponComponent>(entityId);
    ShieldComponent& shieldComp = createComponent<ShieldComponent>(entityId);

    //######### DATA ########//
    situation.position = position;
    situation.rotation = r;

    colliderComp.type = ColliderType::STATIC;


    switch (goType) {
    case GameObjectType::SHIELD:
        colliderComp.collisionLayer = ColliderComponent::Shield;
        colliderComp.layerMasc = ColliderComponent::Player + ColliderComponent::PlayerAttack + ColliderComponent::PlayerShield;
        break;

    case GameObjectType::PLAYER_SHIELD:
        colliderComp.collisionLayer = ColliderComponent::PlayerShield;
        colliderComp.layerMasc = ColliderComponent::Enemy + ColliderComponent::Attack + ColliderComponent::Shield;

        meleeComp.attackSound.soundPath = "Media/Sound/Weapons/shieldZap.wav";
        meleeComp.maxCooldown = 0.7f;
        meleeComp.damage = 10;
        healthComp.maxHealth = 3;
        healthComp.resetHealth();
        situation.noWorldDelete = true;

        break;
    }


    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::SHIELD, goType));
    return entityId;
}


int EntityManager::createPowerUp(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    PowerUpComponent& powerUpComp = createComponent<PowerUpComponent>(entityId);
    //RenderComponent& renderComp = createComponent<RenderComponent>(entityId);

    //######### DATA ########//
    situation.position = position;
    situation.rotation = r;

    // Collider
    colliderComp.collisionLayer = ColliderComponent::Weapon;
    colliderComp.layerMasc = ColliderComponent::Player + ColliderComponent::Wall; //Collides with player and wall
    colliderComp.boundingRoot.bounding = { 0.f, 10.f, 0.f, 10.f };
    colliderComp.type = ColliderType::DYNAMIC;

    // Render component
    //renderComp.sprite = "Media/Images/TaOmA.png";
    //renderComp.spriteRect = { 400, 450, 200, 250 };


    switch (goType) {
    case GameObjectType::POWERUP_SHIELD:
        powerUpComp.type = PowerUpComponent::Shield;
        powerUpComp.colliderIncFactor = 1.5f;
        break;
    }


    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::POWERUP, goType));
    return entityId;
}

// ------------------------------ WORLD CREATION ------------------------------

void EntityManager::createWorld(GameEngine& gameContext, WorldEnum worldName) {

    switch (worldName)
    {
    case WorldEnum::DEBUG:
        // World limits
        WorldData::worldLimits = { 0.f, 10000.f, 0.f, 500.f };

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
