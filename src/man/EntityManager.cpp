#include "EntityManager.hpp"

#include <tools/Utils.hpp>
#include <tools/Sound.hpp>
#include <tools/WorldElementsData.hpp>
#include <tools/MapLoader.hpp>
#include <tools/BackgroundLayer.hpp>
#include <tools/AnimationManager.hpp>

#include <math.h>
#include <random>
#include <eng/GameEngine.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>

#include <iostream>
#include <limits>
#include <fstream> 

#include <tools/json.hpp>
using json = nlohmann::json;

EntityManager::EntityManager() {
}

EntityManager::~EntityManager() {
    // TODO Modify this to not add new components all times
    getComponents<SituationComponent>().clear();
    getComponents<InputComponent>().clear();
    getComponents<VelocityComponent>().clear();
    getComponents<HealthComponent>().clear();
    getComponents<ColliderComponent>().clear();
    getComponents<MeleeWeaponComponent>().clear();
    getComponents<WeaponComponent>().clear();
    getComponents<AttackComponent>().clear();
    getComponents<JumpComponent>().clear();
    getComponents<RenderComponent>().clear();
    getComponents<DistanceWeaponComponent>().clear();
    getComponents<SensorComponent>().clear();
    getComponents<CameraComponent>().clear();
    getComponents<MenuComponent>().clear();
    getComponents<ExplosionAttackComponent>().clear();
    getComponents<BombComponent>().clear();
    getComponents<DodgeComponent>().clear();
    getComponents<SpawnerComponent>().clear();
    getComponents<ShieldComponent>().clear();
    getComponents<PowerUpComponent>().clear();
    getComponents<FuryComponent>().clear();
    getComponents<TextComponent>().clear();
    getComponents<MenuOptionComponent>().clear();
    getComponents<WorldComponent>().clear();
    getComponents<TriggerComponent>().clear();
    getComponents<AnimationComponent>().clear();
    getComponents<OrbitalWeaponComponent>().clear();
    getComponents<AutodeleteComponent>().clear();
    getComponents<TurretComponent>().clear();
    getComponents<GunTurretComponent>().clear();
    getComponents<CheckpointComponent>().clear();
    getComponents<FallWhenTouchedComponent>().clear();

    // AI
    getComponents<AIChaseComponent>().clear();
    getComponents<AIMeleeAtkComponent>().clear();
    getComponents<AIDistanceAtkComponent>().clear();
    getComponents<AITransformationComponent>().clear();
    getComponents<AIDropBombComponent>().clear();
    getComponents<AIPounceComponent>().clear();
    getComponents<AIFlyingChaseComponent>().clear();
    getComponents<AIOrbitalAtkComponent>().clear();
}


Entity &EntityManager::getEntity(int id) {
    return entityMap.at(id);
}

bool EntityManager::existEntity(int id) const{
    if (entityMap.find(id) == entityMap.end())
        return false;

    return true;
}


void EntityManager::eraseEntityByID(int id) {
    // TODO Modify this to not add new components all times
    eraseComponent<SituationComponent>(id);
    eraseComponent<InputComponent>(id);
    eraseComponent<VelocityComponent>(id);
    eraseComponent<HealthComponent>(id);
    eraseComponent<ColliderComponent>(id);
    eraseComponent<MeleeWeaponComponent>(id);
    eraseComponent<WeaponComponent>(id);
    eraseComponent<AttackComponent>(id);
    eraseComponent<JumpComponent>(id);
    eraseComponent<RenderComponent>(id);
    eraseComponent<DistanceWeaponComponent>(id);
    eraseComponent<SensorComponent>(id);
    eraseComponent<CameraComponent>(id);
    eraseComponent<MenuComponent>(id);
    eraseComponent<ExplosionAttackComponent>(id);
    eraseComponent<BombComponent>(id);
    eraseComponent<DodgeComponent>(id);
    eraseComponent<SpawnerComponent>(id);
    eraseComponent<ShieldComponent>(id);
    eraseComponent<PowerUpComponent>(id);
    eraseComponent<FuryComponent>(id);
    eraseComponent<TextComponent>(id);
    eraseComponent<MenuOptionComponent>(id);
    eraseComponent<WorldComponent>(id);
    eraseComponent<TriggerComponent>(id);
    eraseComponent<AnimationComponent>(id);
    eraseComponent<OrbitalWeaponComponent>(id);
    eraseComponent<AutodeleteComponent>(id);
    eraseComponent<TurretComponent>(id);
    eraseComponent<GunTurretComponent>(id);
    eraseComponent<CheckpointComponent>(id);
    eraseComponent<FallWhenTouchedComponent>(id);

    // AI
    eraseComponent<AIChaseComponent>(id);
    eraseComponent<AIMeleeAtkComponent>(id);
    eraseComponent<AIDistanceAtkComponent>(id);
    eraseComponent<AITransformationComponent>(id);
    eraseComponent<AIDropBombComponent>(id);
    eraseComponent<AIPounceComponent>(id);
    eraseComponent<AIFlyingChaseComponent>(id);
    eraseComponent<AIOrbitalAtkComponent>(id);

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
    AnimationComponent& animComp = createComponent<AnimationComponent>(entityId);
    // Create the distance weapon
    if(WorldElementsData::currentWorld != 0)
        Utils::setNormalPistolToEntity(gameContext, entityId);
    else {
        DistanceWeaponComponent& distanceWeaponComp = gameContext.entityMan->createComponent<DistanceWeaponComponent>(entityId);

        distanceWeaponComp.attackBounding = { 0.f, 0.f, 0.f, 0.f };
        distanceWeaponComp.damage = 0;
        distanceWeaponComp.attackGeneralVelociy = 0.f;
        distanceWeaponComp.attackGravity = 0.f;
        distanceWeaponComp.maxCooldown = 4000.f;
        distanceWeaponComp.attackGeneratedType = DistanceWeaponComponent::BULLET;
        distanceWeaponComp.ammo = 0;
        distanceWeaponComp.infiniteAmmo = true;
        distanceWeaponComp.bulletSpreadAngle = 1.f;
        distanceWeaponComp.spawnAttackPos = { 0.f, 0.f };

    }

    //######### DATA ########//
    situation.position = position;
    situation.rotation = r;
    situation.noWorldDelete = true;
    situation.scale = Vector2(0.18f, 0.18f);

    velocityComp.speedX = 120.f;
    velocityComp.gravity = 500.f;

    WorldElementsData::playerId = entityId;

    // Collider
    colliderComp.collisionLayer = ColliderComponent::Player;
    colliderComp.type = ColliderType::DYNAMIC;
    colliderComp.layerMasc = 0xFFF - ColliderComponent::PlayerAttack - ColliderComponent::PlayerShield; //Collides with everything except PlayerAttacks
    colliderComp.boundingRoot.bounding = { 5.f, 45.f, 0.f, 78.f };
    colliderComp.weight = 2.f; // if changed, check turret system, to reset the same

    // Melee
    meleeWeaponComp.attackBounding = { 0.f, 28.f, 20.f, 58.f };
    meleeWeaponComp.damage = 3;
    meleeWeaponComp.attackLifetime = 0.15f;
    meleeWeaponComp.attackSound.soundPath = "./Media/Sound/Weapons/knifeSwing.wav";


    // Render component
    renderComp.sprite = "Media/Images/SpriteSheet _Completo.png";
    renderComp.spriteRect = { 100, 400, 60, 500 };

    // Jump
    jumpComp.impulse = -310.f;
    jumpComp.jumpSound.soundPath = "./Media/Sound/Player/jump.wav";
    jumpComp.jumpSound.volume = 60.f;
    jumpComp.maxCooldown = 0.32f;
    //jumpComp.jumptable = { 500.f, 500.f, 400.f, 400.f, 300.f, 300.f, 200.f, 100.f };
    
    // Sensor
    sensorComp.sensorBounding = {0.f, 50.f, 2.f, 76.f};
    sensorComp.sensorLayerMasc = ColliderComponent::Enemy + ColliderComponent::Turret; // Sensors enemies and turrets

    // Dodge
    dodgeComp.dodgeMaxDuration = 0.3f;
    dodgeComp.dodgeTime = 0.2f;
    dodgeComp.maxCooldown = 1.f;
    dodgeComp.velocityIncrementFactor = 4.f;
    dodgeComp.dodgeSound.soundPath = "./Media/Sound/Player/grunting_8_sean.wav";
    dodgeComp.dodgeSound.volume = 40.f;
    dodgeComp.initDodgeComponent();

    // Health
    heathComp.extraLifes = 1;
    heathComp.recoverTime = 2.f;
    heathComp.damageSound.soundPath = "./Media/Sound/Player/damage_6_sean.wav";
    heathComp.damageSound.volume = 60.f;
    heathComp.deathSound.soundPath = "./Media/Sound/Player/death_10_sean(modified).wav";
    //heathComp.recoverTimeCounter = heathComp.recoverTime;


    // Input (Read Json)
    std::ifstream i("controls.json");
    json jf = json::parse(i);
    inputComp.keyboardControlsMap[Controls::MOVE_LEFT]  = jf["MOVE_LEFT"].get<uint8_t>();
    inputComp.keyboardControlsMap[Controls::MOVE_RIGHT] = jf["MOVE_RIGHT"].get<uint8_t>();
    inputComp.keyboardControlsMap[Controls::MOVE_UP]    = jf["MOVE_UP"].get<uint8_t>();
    inputComp.keyboardControlsMap[Controls::MOVE_DOWN]  = jf["MOVE_DOWN"].get<uint8_t>();
    inputComp.keyboardControlsMap[Controls::ACTION]     = jf["ACTION"].get<uint8_t>();
    inputComp.keyboardControlsMap[Controls::JUMP]       = jf["JUMP"].get<uint8_t>();
    inputComp.keyboardControlsMap[Controls::JOYSTICK_ACTION] = jf["JOYSTICK_ACTION"].get<uint8_t>();
    inputComp.keyboardControlsMap[Controls::JOYSTICK_JUMP]   = jf["JOYSTICK_JUMP"].get<uint8_t>();
    i.close();

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::PLAYER, goType));

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    //situation.sons.emplace_back(createPlayerHand(gameContext, entityId));

    return entityId;
}

int EntityManager::createPlayerHand(GameEngine& gameContext, int playerId) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    RenderComponent& renderComp = createComponent<RenderComponent>(entityId);

    //######### DATA ########//
    situation.position = {0.f, 0.f};
    situation.rotation = 0;
    situation.noWorldDelete = true;
    situation.scale = Vector2(10.f, 10.f);
    situation.father = playerId;

    
    // Render component
    renderComp.sprite = "Media/Images/spawnerSprite.png";
    renderComp.spriteRect = { 0, 20, 0, 20 };

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::PLAYER, GameObjectType::PLAYER_HAND));

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    return entityId;
}


int EntityManager::createAttack(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    ColliderComponent& collider = createComponent<ColliderComponent>(entityId);
    AttackComponent& attack = createComponent<AttackComponent>(entityId);
    RenderComponent& renderComp = createComponent<RenderComponent>(entityId);


    //######### DATA ########//
    situation.position = position;
    situation.rotation = r;

    // Render component
    situation.scale = {0.6f, 0.6f};
    renderComp.sprite = "Media/Images/bullet.png";
    renderComp.spriteRect = { 77, 93, 0, 9 };

    collider.type = ColliderType::NO_SOLID;

    if (goType == GameObjectType::MELEE_ATTACK) {
        renderComp.spriteRect = { 0, 0, 0, 0 };

        collider.collisionLayer = ColliderComponent::Attack;
        collider.layerMasc = ColliderComponent::Player + ColliderComponent::PlayerShield;  // Collides with player only

        attack.type = AttackType::MELEE;
    }
    else if (goType == GameObjectType::DISTANCE_ATTACK) {
        // Render component
        situation.scale = { 0.1f, 0.1f };
        renderComp.sprite = "Media/Images/enemyBullet.png";
        renderComp.spriteRect = { 0, 116, 0, 114 };

        createComponent<VelocityComponent>(entityId);
        collider.collisionLayer = ColliderComponent::Attack;
        collider.layerMasc = ColliderComponent::Player + ColliderComponent::Wall + ColliderComponent::PlayerShield;  // Collides with player and walls

        attack.type = AttackType::DISTANCE;
        // Sound
        attack.hitSound.soundPath = "./Media/Sound/Weapons/BulletHit1.wav";
    }

    else if (goType == GameObjectType::PLAYER_MELEE_ATTACK){
        renderComp.spriteRect = { 0, 0, 0, 0 };

        collider.collisionLayer = ColliderComponent::PlayerAttack;
        collider.layerMasc = ColliderComponent::Enemy + ColliderComponent::Shield + ColliderComponent::Child;  // Collides with enemies only

        attack.type = AttackType::MELEE;
}

    else if (goType == GameObjectType::PLAYER_DISTANCE_ATTACK){
        createComponent<VelocityComponent>(entityId);

        collider.collisionLayer = ColliderComponent::PlayerAttack;
        collider.layerMasc = ColliderComponent::Enemy + ColliderComponent::Wall + ColliderComponent::Shield + ColliderComponent::Child;  // Collides with enemies and walls

        attack.type = AttackType::DISTANCE;
        // Sound
        attack.hitSound.soundPath = "./Media/Sound/Weapons/BulletHit2.wav";
        }

    else if (goType == GameObjectType::EXPLOSION){
        // Render component
        renderComp.sprite = "Media/Images/explosion.png";
        renderComp.spriteRect = { 3655, 3761, 64, 180 };

        situation.scale = { 0.7f, 0.7f };

        createComponent<ExplosionAttackComponent>(entityId);
        attack.type = AttackType::EXPLOSION;
        }

    else if (goType == GameObjectType::PLAYER_EXPLOSION){
        // Render component
        renderComp.sprite = "Media/Images/explosion.png";
        renderComp.spriteRect = { 3655, 3761, 64, 180 };

        situation.scale = { 0.7f, 0.7f };

        createComponent<ExplosionAttackComponent>(entityId);
        attack.type = AttackType::EXPLOSION;
        }

    else if (goType == GameObjectType::PLAYER_LASER){
    createComponent<VelocityComponent>(entityId);

        collider.collisionLayer = ColliderComponent::PlayerAttack;
        collider.layerMasc = ColliderComponent::Enemy + ColliderComponent::Wall + ColliderComponent::Shield + ColliderComponent::Child;  // Collides with enemies and walls

        attack.type = AttackType::LASER;
}

    else if (goType == GameObjectType::LASER) {
        createComponent<VelocityComponent>(entityId);
        collider.collisionLayer = ColliderComponent::Attack;
        collider.layerMasc = ColliderComponent::Player + ColliderComponent::Wall + ColliderComponent::PlayerShield;  // Collides with player and walls

        attack.type = AttackType::LASER;
        }
    

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::ATTACK, goType));

    if (goType == GameObjectType::EXPLOSION || goType == GameObjectType::PLAYER_EXPLOSION) {
        // Animation
        AnimationComponent& animComp = createComponent<AnimationComponent>(entityId);
        AnimationManager::setAnimationToEntity(gameContext, Animation::IDLE, animComp);
    }

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    return entityId;
}


int EntityManager::createWall(GameEngine& gameContext, Vector2 position, Vector2 size, float r, std::string texturePath, GameObjectType goType) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    RenderComponent& renderComp = createComponent<RenderComponent>(entityId);


    //######### DATA ########//
    situation.position = position;
    situation.rotation = r;

    // Render
    renderComp.color = { 255, 255, 255, 255 };
    if (texturePath == "") {
        texturePath = "./Media/Images/Textures/NormalBrick.png";
    }
    renderComp.sprite = texturePath;
    renderComp.spriteRect = {0, size.x, 0, size.y};
    renderComp.isRepeated = true;

    // Collider
    colliderComp.boundingRoot.bounding = { 0.f, size.x, 0.f, size.y };

    switch (goType)
    {
    case GameObjectType::PLATFORM:
        colliderComp.collisionLayer = ColliderComponent::Platform;
        colliderComp.layerMasc = ColliderComponent::Enemy + ColliderComponent::Player + ColliderComponent::Weapon + ColliderComponent::Attack + ColliderComponent::PlayerAttack;
        break;

    case GameObjectType::WALL:
        colliderComp.collisionLayer = ColliderComponent::Wall;
        colliderComp.layerMasc = ColliderComponent::Enemy + ColliderComponent::Player + ColliderComponent::PlayerAttack + ColliderComponent::Weapon + ColliderComponent::Attack; //Collides with player and enemy
        break;

    case GameObjectType::FALLING_WALL:
        colliderComp.collisionLayer = ColliderComponent::Wall;
        colliderComp.layerMasc = ColliderComponent::Enemy + ColliderComponent::Player + ColliderComponent::PlayerAttack + ColliderComponent::Weapon + ColliderComponent::Attack; //Collides with player and enemy

        FallWhenTouchedComponent& fallComp = createComponent<FallWhenTouchedComponent>(entityId);
        fallComp.gravity = 520.f;
        fallComp.objectiveId = WorldElementsData::playerId;
        break;
    }

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::WALL, goType));

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

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
    situation.scale = { 0.18f, 0.18f };

    // Collider
    colliderComp.collisionLayer = ColliderComponent::Enemy;
    colliderComp.layerMasc = ColliderComponent::Player + ColliderComponent::PlayerAttack + ColliderComponent::Wall + ColliderComponent::PlayerShield + ColliderComponent::Platform; //Collides with player and enemy
    colliderComp.type = ColliderType::DYNAMIC;

    // Velocity
    velocityComp.gravity = 230.f;

    // HealthComponent
    healthComp.maxHealth = 5;


    // Add entity to World enemy number
    ++WorldElementsData::enemiesInWorld;

    if (goType == GameObjectType::FIRST_ENEMY) {
        healthComp.maxHealth = 1;
        situation.scale = { 0.27f, 0.27f };

        situation.facing = SituationComponent::Right;

        // Collider
        colliderComp.boundingRoot.bounding = { 0.f, 57.f, 0.f, 90.f };
        colliderComp.weight = 4;

        // Render component
        renderComp.sprite = "Media/Images/FirstEnemy.png";
        renderComp.spriteRect = { 0, 512, 0, 512 };

        situation.facing = SituationComponent::Right;
    }
    else if (goType == GameObjectType::CHASER) {
        velocityComp.speedX = 70.f;

        situation.scale = { 0.27f, 0.27f };
        // Collider
        colliderComp.boundingRoot.bounding = { 0.f, 57.f, 0.f, 90.f };

        // Render component
        renderComp.sprite = "Media/Images/Enemy.png";
        renderComp.spriteRect = { 0, 248, 0, 360 };
        renderComp.color = { 10, 20, 255, 255 };

        healthComp.maxHealth = 3;

        createComponent<AIChaseComponent>(entityId);
    }

    else if (goType == GameObjectType::CHASERJUMPER) {
        velocityComp.speedX = 65.f;

        situation.scale = { 0.27f, 0.27f };
        // Collider
        colliderComp.boundingRoot.bounding = { 0.f, 57.f, 0.f, 90.f };

        // Render component
        renderComp.sprite = "Media/Images/Enemy.png";
        renderComp.spriteRect = { 0, 248, 0, 360 };

        healthComp.maxHealth = 5;

        AIChaseComponent& chaseComp = createComponent<AIChaseComponent>(entityId);
        createComponent<AIMeleeAtkComponent>(entityId);
        JumpComponent& jumpComp = createComponent<JumpComponent>(entityId);
        SensorComponent& sensorComp = createComponent<SensorComponent>(entityId);

        sensorComp.sensorLayerMasc = ColliderComponent::Player + ColliderComponent::Wall;
        sensorComp.sensorBounding = { 25.f, 55.f, 2.f, 60.f };

        jumpComp.impulse = -150.f;

        chaseComp.minDistanceX = rand() % (65 - 46) + 46; // Entre 65 y 46

        MeleeWeaponComponent& meleeWeaponComp = createComponent<MeleeWeaponComponent>(entityId);
        meleeWeaponComp.attackBounding = { 0.f, 34.f, 10.f, 40.f };
        meleeWeaponComp.damage = 2;
        meleeWeaponComp.maxCooldown = 1.5f;
        meleeWeaponComp.attackSound.soundPath = "./Media/Sound/Weapons/slaphit.wav"; ;

        colliderComp.weight = 3.f;
    }
    
    else if(goType == GameObjectType::DISTANCE_ENEMY) {
        velocityComp.speedX = 0.f;

        situation.scale = { 0.27f, 0.27f };
        // Collider
        colliderComp.boundingRoot.bounding = { 0.f, 57.f, 0.f, 90.f };
        
        // Render component
        renderComp.sprite = "Media/Images/Enemy.png";
        renderComp.spriteRect = { 0, 248, 0, 360 };

        healthComp.maxHealth = 5;

        AIDistanceAtkComponent& distanceAIComp = createComponent<AIDistanceAtkComponent>(entityId);
        distanceAIComp.range.x = 400.f;
        distanceAIComp.range.y = 400.f;

        DistanceWeaponComponent& distanceWeaponComp = createComponent<DistanceWeaponComponent>(entityId);
        // Distance
        distanceWeaponComp.attackBounding = { 0.f, 5.f, 0.f, 5.f };
        distanceWeaponComp.damage = 1;
        distanceWeaponComp.attackGeneralVelociy = 300.f;
        distanceWeaponComp.attackGravity = 100.f;
        distanceWeaponComp.maxCooldown = 1.5f;
        distanceWeaponComp.attackGeneratedType = DistanceWeaponComponent::BULLET;
        distanceWeaponComp.attackLifetime = 1.f;
        distanceWeaponComp.bulletSpreadAngle = 5.f;
        distanceWeaponComp.infiniteAmmo = true;
        distanceWeaponComp.spawnAttackPos = { 28.f, 25.f };

        colliderComp.weight = 3.f;
    }

    else if (goType == GameObjectType::DISTANCE_WALKING_ENEMY) {
        velocityComp.speedX = 55.f;

        situation.scale = { 0.27f, 0.27f };
        // Collider
        colliderComp.boundingRoot.bounding = { 0.f, 57.f, 0.f, 90.f };

        // Render component
        renderComp.sprite = "Media/Images/Enemy.png";
        renderComp.spriteRect = { 0, 248, 0, 360 };
        renderComp.color = { 238, 249, 14, 255 };

        healthComp.maxHealth = 4;

        AIDistanceAtkComponent& distanceAIComp = createComponent<AIDistanceAtkComponent>(entityId);
        distanceAIComp.range.x = 240.f;
        distanceAIComp.range.y = 240.f;

        DistanceWeaponComponent& distanceWeaponComp = createComponent<DistanceWeaponComponent>(entityId);
        // Distance
        distanceWeaponComp.attackBounding = { 0.f, 5.f, 0.f, 5.f };
        distanceWeaponComp.damage = 1;
        distanceWeaponComp.attackGeneralVelociy = 300.f;
        distanceWeaponComp.attackGravity = 100.f;
        distanceWeaponComp.maxCooldown = 1.f;
        distanceWeaponComp.attackGeneratedType = DistanceWeaponComponent::BULLET;
        distanceWeaponComp.attackLifetime = 1.f;
        distanceWeaponComp.bulletSpreadAngle = 5.f;
        distanceWeaponComp.infiniteAmmo = true;

        colliderComp.weight = 3.f;

        AIChaseComponent& chaseComp = createComponent<AIChaseComponent>(entityId);
        JumpComponent& jumpComp = createComponent<JumpComponent>(entityId);
        SensorComponent& sensorComp = createComponent<SensorComponent>(entityId);

        sensorComp.sensorLayerMasc = ColliderComponent::Wall;
        sensorComp.sensorBounding = { 25.f, 55.f, 2.f, 60.f };

        jumpComp.impulse = -150.f;

        chaseComp.minDistanceX = rand() % (230 - 100) + 100; // Entre 100 y 230
    }

    else if (goType == GameObjectType::TRANSFORM_ENEMY) {
        velocityComp.speedX = 100.f;

        // Collider
        colliderComp.boundingRoot.bounding = { 0.f, 18.f, 0.f, 49.f };

        // Render component
        renderComp.sprite = "Media/Images/Personajes/Alien camuflado/Nino.png";
        renderComp.spriteRect = { 0, 116, 0, 274 };

        healthComp.maxHealth = 10;

        JumpComponent& jumpComp = createComponent<JumpComponent>(entityId);
        SensorComponent& sensorComp = createComponent<SensorComponent>(entityId);

        sensorComp.sensorLayerMasc = ColliderComponent::Player + ColliderComponent::Wall;
        sensorComp.sensorBounding = { 25.f, 75.f, 2.f, 48.f };

        jumpComp.impulse = -150.f;

        MeleeWeaponComponent& meleeWeaponComp = createComponent<MeleeWeaponComponent>(entityId);
        meleeWeaponComp.attackBounding = { 0.f, 50.f, 70.f, 175.f };
        meleeWeaponComp.damage = 2;
        meleeWeaponComp.maxCooldown = 1.2f;
        meleeWeaponComp.attackSound.soundPath = "./Media/Sound/Enemies/MonsterRoar.wav";

        AITransformationComponent& transformComp = createComponent<AITransformationComponent>(entityId);
        transformComp.newBoundingRoot.bounding = { 2.f, 90.f, 0.f, 180.f };
        transformComp.newColor = { 255, 255, 255, 255 };
        transformComp.newScale = { 0.18f, 0.18f };
        transformComp.newSprite = "Media/Images/Personajes/Alien camuflado/AlienTransformado.png";
        transformComp.newSpriteRect = { 0, 520, 0, 1016 };
        transformComp.range = { 120.f, 100.f };
        transformComp.transformationSound.soundPath = "./Media/Sound/Enemies/dinosaur-monster-roar-1976.wav";
    }

    else if (goType == GameObjectType::BOMBER_ENEMY) {
        velocityComp.speedX = 80.f;
        velocityComp.gravity = 0.f;

        // Collider
        colliderComp.boundingRoot.bounding = { 2.f, 71.f, 2.f, 80.f };

        // Render component
        renderComp.sprite = "Media/Images/Personajes/alien volador/AlienVolador.png";
        renderComp.spriteRect = { 0, 399, 0, 464 };

        healthComp.maxHealth = 9;

        AIFlyingChaseComponent& flyingChaseComp = createComponent<AIFlyingChaseComponent>(entityId);

        flyingChaseComp.maxHeigtht = 230.f;
        flyingChaseComp.minHeigtht = 200.f;
        flyingChaseComp.minDistanceX = rand() % (38 - 10) + 10; // Entre 65 y 46


        DistanceWeaponComponent& distanceWeaponComp = createComponent<DistanceWeaponComponent>(entityId);

        distanceWeaponComp.attackBounding = { 0.f, 20.f, 0.f, 20.f };
        distanceWeaponComp.damage = 3;
        distanceWeaponComp.attackGeneralVelociy = 0.f;
        distanceWeaponComp.attackGravity = 100.f;
        distanceWeaponComp.maxCooldown = 0.5f;
        distanceWeaponComp.attackLifetime = 0.3f;
        distanceWeaponComp.attackGeneratedType = DistanceWeaponComponent::BOMB;

        distanceWeaponComp.attackSound.soundPath = "Media/Sound/Weapons/silbidoBombaCayendo.wav";

        distanceWeaponComp.explosionExpansion = 90.f;
        distanceWeaponComp.explosionTime = 0.5f;
        distanceWeaponComp.startActivated = false;
        distanceWeaponComp.spawnAttackPos = { 26.f, 40.f };

        AIDropBombComponent& dropBombComp = createComponent<AIDropBombComponent>(entityId);
        dropBombComp.maxCooldown = 2.f;
    }

    else if (goType == GameObjectType::FLYING_SHOOTER_ENEMY) {
    velocityComp.speedX = 65.f;
    velocityComp.gravity = 0.f;

    // Collider
    colliderComp.boundingRoot.bounding = { 2.f, 71.f, 2.f, 80.f };

    // Render component
    renderComp.sprite = "Media/Images/Personajes/alien volador/AlienVolador.png";
    renderComp.spriteRect = { 0, 399, 0, 464 };
    renderComp.color = { 238, 249, 14, 255 };

    healthComp.maxHealth = 5;

    AIFlyingChaseComponent& flyingChaseComp = createComponent<AIFlyingChaseComponent>(entityId);

    flyingChaseComp.maxHeigtht = 230.f;
    flyingChaseComp.minHeigtht = 200.f;
    flyingChaseComp.minDistanceX = rand() % (38 - 10) + 10; // Entre 65 y 46

    AIDistanceAtkComponent& distanceAIComp = createComponent<AIDistanceAtkComponent>(entityId);
    distanceAIComp.range.x = 175.f;
    distanceAIComp.range.y = 235.f;

    DistanceWeaponComponent& distanceWeaponComp = createComponent<DistanceWeaponComponent>(entityId);
    // Distance
    distanceWeaponComp.attackBounding = { 0.f, 5.f, 0.f, 5.f };
    distanceWeaponComp.damage = 1;
    distanceWeaponComp.attackGeneralVelociy = 253.f;
    distanceWeaponComp.attackGravity = 10.f;
    distanceWeaponComp.maxCooldown = 1.f;
    distanceWeaponComp.attackGeneratedType = DistanceWeaponComponent::BULLET;
    distanceWeaponComp.attackLifetime = 1.5f;
    distanceWeaponComp.bulletSpreadAngle = 15.f;
    distanceWeaponComp.infiniteAmmo = true;
    }

    else if (goType == GameObjectType::POUNCER_ENEMY) {
        velocityComp.speedX = 70.f;

        // Collider
        colliderComp.boundingRoot.bounding = { 0.f, 52.f, 0.f, 67.f };

        // Render component
        renderComp.sprite = "Media/Images/Personajes/Alien ara�a/AlienArana.png";
        renderComp.spriteRect = { 0, 297, 0, 381 };
        renderComp.color = { 20, 20, 30, 255 };

        healthComp.maxHealth = 4;


        createComponent<AIMeleeAtkComponent>(entityId);
        JumpComponent& jumpComp = createComponent<JumpComponent>(entityId);
        SensorComponent& sensorComp = createComponent<SensorComponent>(entityId);
        AIPounceComponent& pounceComp = createComponent<AIPounceComponent>(entityId);

        sensorComp.sensorLayerMasc = ColliderComponent::Player + ColliderComponent::Wall;
        sensorComp.sensorBounding = { 25.f, 60.f, 2.f, 65.f };

        jumpComp.impulse = -100.f;
        jumpComp.maxCooldown = 1.f;
        jumpComp.jumpSound.soundPath = "./Media/Sound/Enemies/spiderJump.wav";


        MeleeWeaponComponent& meleeWeaponComp = createComponent<MeleeWeaponComponent>(entityId);
        meleeWeaponComp.attackBounding = { 0.f, 20.f, 10.f, 40.f };
        meleeWeaponComp.damage = 2;
        meleeWeaponComp.maxCooldown = 1.5f;
        meleeWeaponComp.attackSound.soundPath = "./Media/Sound/Enemies/spiderAttackShort.wav";


        pounceComp.range.x = 200.f;
        pounceComp.range.y = 30.f;
        pounceComp.velocityIncFactor = 4.7f;
        pounceComp.maxCooldown = 2.f;
    }

    else if (goType == GameObjectType::ENEMY_SPIDER) {
        velocityComp.speedX = 80.f;

        // Collider
        colliderComp.boundingRoot.bounding = { 0.f, 52.f, 0.f, 67.f };
        
        // Render component
        renderComp.sprite = "Media/Images/Personajes/Alien ara�a/AlienArana.png";
        renderComp.spriteRect = { 0, 297, 0, 381 };

        healthComp.maxHealth = 8;


        createComponent<AIMeleeAtkComponent>(entityId);
        JumpComponent& jumpComp = createComponent<JumpComponent>(entityId);
        SensorComponent& sensorComp = createComponent<SensorComponent>(entityId);
        AIPounceComponent& pounceComp = createComponent<AIPounceComponent>(entityId);

        sensorComp.sensorLayerMasc = ColliderComponent::Player + ColliderComponent::Wall;
        sensorComp.sensorBounding = { 25.f, 60.f, 2.f, 65.f };

        jumpComp.impulse = -150.f;
        jumpComp.maxCooldown = 2.f;
        jumpComp.cooldown = 2.f;
        jumpComp.jumpSound.soundPath = "./Media/Sound/Enemies/spiderJump.wav";

        MeleeWeaponComponent& meleeWeaponComp = createComponent<MeleeWeaponComponent>(entityId);
        meleeWeaponComp.attackBounding = { 0.f, 20.f, 10.f, 40.f };
        meleeWeaponComp.damage = 1;
        meleeWeaponComp.maxCooldown = 1.2f;
        meleeWeaponComp.attackSound.soundPath = "./Media/Sound/Enemies/spiderAttackShort.wav";

        pounceComp.range.x = 200.f;
        pounceComp.range.y = 30.f;
        pounceComp.velocityIncFactor = 4.7f;
        pounceComp.maxCooldown = 1.2f;
        pounceComp.cooldown = 1.2f;
        pounceComp.isStickyPouncer = true;

        colliderComp.weight = 3.f;
    }

    healthComp.resetHealth(); // Reset health to set the current health the same as maxHealth


    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::ENEMY, goType));

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

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
    colliderComp.layerMasc = ColliderComponent::Player + ColliderComponent::Wall + ColliderComponent::Platform; //Collides with player and wall
    colliderComp.boundingRoot.bounding = { 0.f, 45.6f, 0.f, 19.5f };
    colliderComp.type = ColliderType::DYNAMIC;

    // Render component
    renderComp.sprite = "Media/Images/TaOmA.png";
    renderComp.spriteRect = { 400, 450, 200, 250 };

    // Velocity
    velocityComp.gravity = 80.f;
    velocityComp.speedX  = 0.f;



    if (goType == GameObjectType::M4) {
        DistanceWeaponComponent& distanceWeaponComp = createComponent<DistanceWeaponComponent>(entityId);

        distanceWeaponComp.attackBounding = { 0.f, 5.f, 0.f, 5.f };
        distanceWeaponComp.damage = 1;
        distanceWeaponComp.attackGeneralVelociy = 900.f;
        distanceWeaponComp.attackGravity = 100.f;
        distanceWeaponComp.maxCooldown = 0.15f;
        distanceWeaponComp.attackLifetime = 1.5f;
        distanceWeaponComp.attackGeneratedType = DistanceWeaponComponent::BULLET;
        distanceWeaponComp.ammo = 70;
        distanceWeaponComp.infiniteAmmo = false;
        distanceWeaponComp.bulletSpreadAngle = 5.f;
        distanceWeaponComp.spawnAttackPos = { 20.f, 39.f };

        distanceWeaponComp.attackSound.soundPath = "Media/Sound/Weapons/M4A1_Single-Kibblesbob-8540445.wav";

        renderComp.sprite = "Media/Images/m4.png";
        renderComp.spriteRect = { 0, 152, 0, 65 };

        situation.scale = { 0.3f, 0.3f };

        WorldElementsData::worldDistanceWeapons.push_back(entityId);
    }

    else if (goType == GameObjectType::KNIFE) {
        MeleeWeaponComponent& meleeWeaponComp = createComponent<MeleeWeaponComponent>(entityId);

        meleeWeaponComp.attackBounding = { 0.f, 20.f, 10.f, 40.f };
        meleeWeaponComp.damage = 4;
        meleeWeaponComp.maxCooldown = 1.f;
        meleeWeaponComp.attackLifetime = 0.1f;
        meleeWeaponComp.spawnAttackPos = { 20.f, 39.f };

        meleeWeaponComp.attackSound.soundPath = "Media/Sound/GE_KF7_Soviet.wav";


        WorldElementsData::worldMeleeWeapons.push_back(entityId);
    }

    else if (goType == GameObjectType::GRENADE_LAUNCHER) {
        DistanceWeaponComponent& distanceWeaponComp = createComponent<DistanceWeaponComponent>(entityId);

        distanceWeaponComp.attackBounding = { 0.f, 20.f, 0.f, 20.f };
        distanceWeaponComp.damage = 10;
        distanceWeaponComp.attackGeneralVelociy = 650.f;
        distanceWeaponComp.attackGravity = 100.f;
        distanceWeaponComp.maxCooldown = 1.f;
        distanceWeaponComp.attackLifetime = 0.5f;
        distanceWeaponComp.attackGeneratedType = DistanceWeaponComponent::BOMB;
        distanceWeaponComp.ammo = 8;
        distanceWeaponComp.infiniteAmmo = false;
        distanceWeaponComp.spawnAttackPos = { 20.f, 39.f };

        distanceWeaponComp.attackSound.soundPath = "Media/Sound/Weapons/grenadeLauncherSound.wav";

        distanceWeaponComp.explosionExpansion = 90.f;
        distanceWeaponComp.explosionTime = 0.2f;
        distanceWeaponComp.startActivated = false;

        renderComp.sprite = "Media/Images/grenade_launcher.png";
        renderComp.spriteRect = { 0, 152, 0, 65 };

        situation.scale = { 0.3f, 0.3f };

        WorldElementsData::worldDistanceWeapons.push_back(entityId);
    }
    
    else if (goType == GameObjectType::LASER_GUN) {
        DistanceWeaponComponent& distanceWeaponComp = createComponent<DistanceWeaponComponent>(entityId);

        distanceWeaponComp.attackBounding = { 0.f, 500.f, 0.f, 5.f };
        distanceWeaponComp.damage = 2;
        distanceWeaponComp.attackGeneralVelociy = 0.f;
        distanceWeaponComp.attackGravity = 0.f;
        distanceWeaponComp.maxCooldown = 0.6f;
        distanceWeaponComp.attackLifetime = 0.6f;
        distanceWeaponComp.attackGeneratedType = DistanceWeaponComponent::LASER;
        distanceWeaponComp.ammo = 30;
        distanceWeaponComp.infiniteAmmo = false;
        distanceWeaponComp.spawnAttackPos = { 20.f, 39.f };
        distanceWeaponComp.attackSound.soundPath = "Media/Sound/Weapons/laserShot.wav";

        renderComp.sprite = "Media/Images/laser_gun.png";
        renderComp.spriteRect = { 0, 152, 0, 65 };

        situation.scale = { 0.3f, 0.3f };

        WorldElementsData::worldDistanceWeapons.push_back(entityId);
    }

    else if (goType == GameObjectType::SHOTGUN) {
        DistanceWeaponComponent& distanceWeaponComp = createComponent<DistanceWeaponComponent>(entityId);

        distanceWeaponComp.attackBounding = { 0.f, 2.f, 0.f, 2.f };
        distanceWeaponComp.damage = 1;
        distanceWeaponComp.attackGeneralVelociy = 900.f;
        distanceWeaponComp.attackGravity = 0.f;
        distanceWeaponComp.maxCooldown = 0.7f;
        distanceWeaponComp.attackLifetime = 0.3f;
        distanceWeaponComp.attackGeneratedType = DistanceWeaponComponent::BULLET;
        distanceWeaponComp.ammo = 7;
        distanceWeaponComp.infiniteAmmo = false;
        distanceWeaponComp.numberOfShells = 4;
        distanceWeaponComp.bulletSpreadAngle = 12.f;
        distanceWeaponComp.spawnAttackPos = { 20.f, 39.f };
        distanceWeaponComp.attackSound.soundPath = "Media/Sound/Weapons/shotgunShot.wav";

        renderComp.sprite = "Media/Images/shotgun.png";
        renderComp.spriteRect = { 0, 152, 0, 65 };

        situation.scale = {0.3f, 0.3f};

        WorldElementsData::worldDistanceWeapons.push_back(entityId);
    } 
    else if (goType == GameObjectType::PISTOL) {
    Utils::setNormalPistolToEntity(gameContext, entityId);

    renderComp.sprite = "Media/Images/shotgun.png";
    renderComp.spriteRect = { 0, 1, 0, 1 };

    situation.scale = { 0.3f, 0.3f };

    WorldElementsData::worldDistanceWeapons.push_back(entityId);
    }

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::WEAPON, goType));

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    return entityId;
}


int EntityManager::createCamera(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();

    WorldElementsData::activeCameraId = entityId;

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    CameraComponent& cameraComp   = createComponent<CameraComponent>(entityId);
    VelocityComponent& velocityComp = createComponent<VelocityComponent>(entityId);


    //######### DATA ########//
    situation.position = position;
    situation.rotation = r;
    situation.noWorldDelete = true;

    // Camera
    cameraComp.viewRect = {600, 400 };
    cameraComp.zoom = 1.4f;
    cameraComp.offset = { 10.f, 70.f};
    cameraComp.cameraAdvancement = 150.f;

    // Velocity
    velocityComp.gravity = 0.f;
    velocityComp.maxVy = 10000.f;
    velocityComp.minVy = -10000.f;
    velocityComp.speedX = 1000.f;


    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::CAMERA, goType));

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createCamera(gameContext, entityId);

    return entityId;
}


int EntityManager::createBomb(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    BombComponent& bombComp = createComponent<BombComponent>(entityId);
    createComponent<VelocityComponent>(entityId); 
    RenderComponent& renderComp = createComponent<RenderComponent>(entityId);

    //######### DATA ########//
    situation.position = position;
    situation.rotation = r;
    situation.scale = { 0.35f, 0.35f };

    // Render component
    renderComp.sprite = "Media/Images/grenade.png";
    renderComp.spriteRect = { 0, 144, 1, 27 };

    colliderComp.type = ColliderType::DYNAMIC;

    switch (goType) {
        case GameObjectType::BOMB:
            colliderComp.collisionLayer = ColliderComponent::Attack;
            colliderComp.layerMasc = ColliderComponent::Player + ColliderComponent::Wall + ColliderComponent::PlayerShield + ColliderComponent::Platform;
            bombComp.explosionSound.soundPath = "Media/Sound/Weapons/alienExplosion.wav";
            break;

        case GameObjectType::PLAYER_BOMB:
            colliderComp.collisionLayer = ColliderComponent::PlayerAttack;
            colliderComp.layerMasc = ColliderComponent::Enemy + ColliderComponent::Wall + ColliderComponent::Shield + /*ColliderComponent::Platform +*/ ColliderComponent::Child;
            bombComp.explosionSound.soundPath = "Media/Sound/Weapons/explosion.wav";
            break;
    }
    

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::BOMB, goType));

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

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
    situation.position.y -= 30;
    situation.rotation = r;
    situation.scale = { 0.25f, 0.5f };

    // Render component
    renderComp.sprite = "Media/Images/spawner.png";
    renderComp.spriteRect = { 0, 200, 0, 200 };

    // Collider
    colliderComp.collisionLayer = ColliderComponent::Enemy; // temporal
    colliderComp.layerMasc = ColliderComponent::Player + ColliderComponent::PlayerAttack + ColliderComponent::Wall + ColliderComponent::PlayerShield + ColliderComponent::Platform; //Collides with player and enemy
    colliderComp.boundingRoot.bounding = { 0.f, 50.f, 0.f, 100.f };
    colliderComp.type = ColliderType::STATIC;
    

    // Add entity to World enemy number
    ++WorldElementsData::enemiesInWorld;

    switch (goTypeToSpawn) {
    case GameObjectType::CHASERJUMPER:

        // Spawner
        spawnComp.maxCooldown = 5.f;
        spawnComp.range = { 200.f, 200.f };
        spawnComp.spawnObjectsType = GameObjectType::CHASERJUMPER;
        spawnComp.spawnEntitiesType = EntityType::ENEMY;

        // Health
        healthComp.maxHealth = 5;

        break;     

    case GameObjectType::ENEMY_SPIDER:

        // Spawner
        spawnComp.maxCooldown = 5.f;
        spawnComp.range = { 200.f, 200.f };
        spawnComp.spawnObjectsType = GameObjectType::ENEMY_SPIDER;
        spawnComp.spawnEntitiesType = EntityType::ENEMY;

        // Health
        healthComp.maxHealth = 5;

        break;

    default:
        spawnComp.spawnObjectsType = goTypeToSpawn;
        spawnComp.spawnEntitiesType = EntityType::ENEMY;
        break;
    }

    // Init health
    healthComp.resetHealth();
    

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::SPAWNER, GameObjectType::SPAWNER));

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    return entityId;
}


int EntityManager::createShield(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    RenderComponent& renderComp = createComponent<RenderComponent>(entityId);
    HealthComponent& healthComp = createComponent<HealthComponent>(entityId);
    MeleeWeaponComponent& meleeComp = createComponent<MeleeWeaponComponent>(entityId);
    ShieldComponent& shieldComp = createComponent<ShieldComponent>(entityId);

    //######### DATA ########//
    situation.position = position;
    situation.rotation = r;

    renderComp.sprite = "./Media/Images/shield.png";
    renderComp.spriteRect = { 0, 56, 0, 110 };


    colliderComp.type = ColliderType::NO_SOLID;


    switch (goType) {
    case GameObjectType::SHIELD:
        colliderComp.collisionLayer = ColliderComponent::Shield;
        colliderComp.layerMasc = ColliderComponent::Player + ColliderComponent::PlayerAttack + ColliderComponent::PlayerShield + ColliderComponent::Wall;
        break;

    case GameObjectType::PLAYER_SHIELD:
        colliderComp.collisionLayer = ColliderComponent::PlayerShield;
        colliderComp.layerMasc = ColliderComponent::Enemy + ColliderComponent::Attack + ColliderComponent::Shield + ColliderComponent::Wall;

        meleeComp.attackSound.soundPath = "Media/Sound/Weapons/shieldZap.wav";
        meleeComp.maxCooldown = 1.5f;
        meleeComp.damage = 1;
        healthComp.maxHealth = 5;
        healthComp.resetHealth();
        situation.noWorldDelete = true;

        break;
    }

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::SHIELD, goType));

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    return entityId;
}


int EntityManager::createDrone(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    HealthComponent& healthComp = createComponent<HealthComponent>(entityId);
    VelocityComponent& velocityComp = createComponent<VelocityComponent>(entityId);
    DistanceWeaponComponent& distanceWeaponComp = createComponent<DistanceWeaponComponent>(entityId);
    AIFlyingChaseComponent& flyingChaseComp = createComponent<AIFlyingChaseComponent>(entityId);
    AIDistanceAtkComponent& distanceAIComp = createComponent<AIDistanceAtkComponent>(entityId);
    SensorComponent& sensorComp = createComponent<SensorComponent>(entityId);

    //######### DATA ########//
    situation.position = position;
    situation.rotation = r;

    colliderComp.type = ColliderType::DYNAMIC;
    colliderComp.boundingRoot.bounding = { 0.f, 30.f, 0.f, 16.f };

    flyingChaseComp.maxHeigtht = 55.f;
    flyingChaseComp.minHeigtht = 50.f;
    flyingChaseComp.minDistanceX = 10.f;

    AutodeleteComponent& deleteComp = createComponent<AutodeleteComponent>(entityId);
    deleteComp.timeToDelete = 50.f;

    switch (goType) {
    case GameObjectType::DRONE_FRIEND:
        

        situation.noWorldDelete = false;

        colliderComp.collisionLayer = ColliderComponent::Player;
        colliderComp.layerMasc = ColliderComponent::Attack + ColliderComponent::Enemy;

        velocityComp.speedX = 80.f;
        velocityComp.gravity = 0.f;

        healthComp.maxHealth = 3;

        distanceWeaponComp.attackBounding = { 0.f, 5.f, 0.f, 5.f };
        distanceWeaponComp.damage = 1;
        distanceWeaponComp.attackGeneralVelociy = 800.f;
        distanceWeaponComp.attackGravity = 0.f;
        distanceWeaponComp.maxCooldown = 0.4f;
        distanceWeaponComp.attackLifetime = 1.5f;
        distanceWeaponComp.attackGeneratedType = DistanceWeaponComponent::BULLET;
        distanceWeaponComp.attackSound.soundPath = "Media/Sound/GE_KF7_Soviet.wav";

        distanceAIComp.range.x = 270.f;
        distanceAIComp.range.y = 270.f;

        sensorComp.sensorBounding = {-distanceAIComp.range.x, distanceAIComp.range.x, -distanceAIComp.range.y, distanceAIComp.range.y };  // The same bounding as the distanceAIComp
        sensorComp.sensorLayerMasc = ColliderComponent::Enemy;

        flyingChaseComp.objectiveId = WorldElementsData::playerId;

        WorldElementsData::playerDroneId = entityId;

        break;

    case GameObjectType::DRONE_ENEMY:

        // Add entity to World enemy number
        ++WorldElementsData::enemiesInWorld;

        break;
    }

    healthComp.resetHealth();

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::DRONE, goType));
    return entityId;
}


int EntityManager::createPowerUp(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    PowerUpComponent& powerUpComp = createComponent<PowerUpComponent>(entityId);
    RenderComponent& renderComp = createComponent<RenderComponent>(entityId);

    //######### DATA ########//
    situation.position = position;
    situation.rotation = r;

    // Collider
    colliderComp.collisionLayer = ColliderComponent::Weapon;
    colliderComp.layerMasc = ColliderComponent::Player; //Collides with player and wall
    colliderComp.boundingRoot.bounding = { 0.f, 30.f, 0.f, 30.f };
    colliderComp.type = ColliderType::NO_SOLID;


    switch (goType) {
    case GameObjectType::POWERUP_SHIELD:
        powerUpComp.type = PowerUpComponent::Shield;
        powerUpComp.shieldColliderIncFactor = 1.4f;

        // Render component
        renderComp.sprite = "Media/Images/Powerups/Shield.png";
        renderComp.spriteRect = { 0, 30, 0, 30 };
        break;

    case GameObjectType::POWERUP_FURY:
        powerUpComp.type = PowerUpComponent::Fury;
        powerUpComp.furyColor = { 255, 0, 0, 255 };
        powerUpComp.furyTimersSpeedIncFactor = 0.7f;
        powerUpComp.furySpeedIncFactor = 1.5f;
        powerUpComp.furyTotalLifeTime = 10.f;

        // Render component
        renderComp.sprite = "Media/Images/Powerups/Fury.png";
        renderComp.spriteRect = { 0, 30, 0, 30 };
        break;

    case GameObjectType::POWERUP_EXTRA_LIFE:
        powerUpComp.type = PowerUpComponent::ExtraLife;

        // Render component
        renderComp.sprite = "Media/Images/Powerups/ExtraLife.png";
        renderComp.spriteRect = { 0, 30, 0, 30 };
        break;

    case GameObjectType::POWERUP_DRONE:
        powerUpComp.type = PowerUpComponent::Drone;

        // Render component
        renderComp.sprite = "Media/Images/Powerups/Drone.png";
        renderComp.spriteRect = { 0, 30, 0, 30 };
        break;
    }

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::POWERUP, goType));

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    
    return entityId;
}


int EntityManager::createTrigger(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation   = createComponent<SituationComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    TriggerComponent& triggerComp   = createComponent<TriggerComponent>(entityId);

    //######### DATA ########//
    situation.position = position;
    situation.rotation = r;

    // Collider
    colliderComp.collisionLayer = ColliderComponent::Trigger;
    colliderComp.layerMasc = ColliderComponent::Player; //Collides with player and wall
    colliderComp.type = ColliderType::NO_SOLID;

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::TRIGGER, goType));
    return entityId;
}


int EntityManager::createDamagePlatform(GameEngine& gameContext, Vector2 position, Vector2 size, uint16_t damage, GameObjectType goType) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    AttackComponent& attack = createComponent<AttackComponent>(entityId);

    //######### DATA ########//
    situation.position = position;

    // Collider
    colliderComp.collisionLayer = ColliderComponent::Wall;
    colliderComp.layerMasc = ColliderComponent::Player + ColliderComponent::Enemy + ColliderComponent::PlayerShield + ColliderComponent::Shield; //Collides with player and wall
    colliderComp.type = ColliderType::STATIC;
    colliderComp.boundingRoot.bounding = {0.f, size.x, 0.f, size.y};
    
    // Attack
    attack.damage = damage;
    attack.maxLifetime = std::numeric_limits<float>::max();
    attack.type = AttackType::DAMAGE_PLATFORM;
    attack.resetDamagedEntTime = 1.f;

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::DAMAGE_PLATFORM, goType));
    return entityId;
}


int EntityManager::createOrbitalMarker(GameEngine& gameContext, Vector2 position, GameObjectType goType) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    AutodeleteComponent& autodelete = createComponent<AutodeleteComponent>(entityId);
    RenderComponent& renderComp = createComponent<RenderComponent>(entityId);

    //######### DATA ########//
    situation.position = position;
    situation.noWorldDelete = true;

    // Render component
    renderComp.sprite = "Media/Images/OrbitalMarker.png";


    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::ORBITAL_MARKER, GameObjectType::ORBITAL_MARKER));

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    return entityId;
}


int EntityManager::createOrbitalStrikerEnemy(GameEngine& gameContext, Vector2 position, GameObjectType goType) {
    int entityId = Entity::getCurrentId();

    createComponent<AIOrbitalAtkComponent>(entityId);
    OrbitalWeaponComponent& orbitalWeapon = createComponent<OrbitalWeaponComponent>(entityId);
    SituationComponent& situation = createComponent<SituationComponent>(entityId);

    //######### DATA ########//
    situation.position = position;
    situation.noWorldDelete = false;

    orbitalWeapon.attackBounding = { 0.f, 50.f, 0.f, 0.f };
    orbitalWeapon.markerBounding = { 0.f, 30.f, 0.f, 0.f };
    orbitalWeapon.attackLifetime = 0.5f;
    orbitalWeapon.damage = 2;
    orbitalWeapon.generateAttackTime = 1.3f;
    orbitalWeapon.maxCooldown = 4.f;
    orbitalWeapon.cooldown = orbitalWeapon.maxCooldown;

    orbitalWeapon.markerSound.soundPath = "Media/Sound/Weapons/loadLaser.wav";
    orbitalWeapon.attackSound.soundPath = "Media/Sound/Weapons/alienExplosion.wav";

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::ORBITAL_STRIKER, GameObjectType::ORBITAL_STRIKER));
    return entityId;
}



void EntityManager::createTurret(GameEngine& gameContext, Vector2 position, uint8_t facing) {
    int turretGun = createTurretGun(gameContext, position, facing, GameObjectType::TURRET_GUN);
    int turretText = createText(gameContext, { 0.f, 0.f }, 0.f, "Press ACTION to use turret", { 0, 0, 0, 0 }, false, 10);
    createTurretPlatform(gameContext, position, facing, turretGun, turretText, GameObjectType::TURRET_PLATFORM);
}

int EntityManager::createTurretGun(GameEngine& gameContext, Vector2 position, uint8_t facing,GameObjectType goType) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    RenderComponent& renderComp = createComponent<RenderComponent>(entityId);
    DistanceWeaponComponent& distanceWeaponComp = createComponent<DistanceWeaponComponent>(entityId);
    GunTurretComponent& gunTurretComp = createComponent<GunTurretComponent>(entityId);

    //######### DATA ########//
    position.x += 10.f;
    position.y += 20.f;
    situation.position = position;
    situation.facing = facing;
    situation.noWorldDelete = true;
    situation.scale = {0.07f, 0.07f};

    // Render
    renderComp.sprite = "Media/PNG/Torreta/torreta.png";
    renderComp.spriteRect = { 443, 1818, 884, 1051 };

    // Distance Weapon
    distanceWeaponComp.attackBounding = { 0.f, 5.f, 0.f, 5.f };
    distanceWeaponComp.damage = 1;
    distanceWeaponComp.attackGeneralVelociy = 900.f;
    distanceWeaponComp.attackGravity = 0.f;
    distanceWeaponComp.maxCooldown = 0.11f;
    distanceWeaponComp.attackLifetime = 0.65f;
    distanceWeaponComp.attackGeneratedType = DistanceWeaponComponent::BULLET;
    distanceWeaponComp.infiniteAmmo = true;
    distanceWeaponComp.bulletSpreadAngle = 5.f;
    distanceWeaponComp.spawnAttackPos = {0.f, 0.f};

    distanceWeaponComp.attackSound.soundPath = "Media/Sound/Weapons/M4A1_Single-Kibblesbob-8540445.wav";

    // Gun Turret
    gunTurretComp.currentRotation = 0.f;
    if (facing == SituationComponent::Left) {
        gunTurretComp.currentRotation = 180.f;
    }
    gunTurretComp.gunRotationSpeed = 80.f;
    gunTurretComp.maxRotation = 90.f;
    gunTurretComp.minRotation = -10.f;

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::TURRET, GameObjectType::TURRET_GUN));

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    return entityId;
}

int EntityManager::createTurretPlatform(GameEngine& gameContext, Vector2 position, uint8_t facing, int turretGun, int turretText, GameObjectType goType) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    TurretComponent& turretComp = createComponent<TurretComponent>(entityId);
    HealthComponent& healthComp = createComponent<HealthComponent>(entityId);
    RenderComponent& renderComp = createComponent<RenderComponent>(entityId);

    //######### DATA ########//
    situation.position = position;
    situation.facing = facing;
    situation.scale = { 0.07f, 0.07f };

    // Render
    renderComp.sprite = "Media/PNG/Torreta/apoyo y escudo.png";
    renderComp.spriteRect = { 475, 1220, 620, 1831 };

    // Collider
    colliderComp.collisionLayer = ColliderComponent::Turret;
    colliderComp.layerMasc = ColliderComponent::NoLayer; //doesn't collide SI SE CAMBIA, CAMBIAR TAMBIEN EN EL TURRET SYSTEM
    colliderComp.type = ColliderType::NO_SOLID;
    colliderComp.boundingRoot.bounding = { 0.f, 40.f, 0.f, 30.f };

    // Turret
    turretComp.rotationVelocity = 30.f;
    turretComp.offsetX = 30.f;
    turretComp.turretGunID = turretGun;
    turretComp.textID = turretText;

    // Health
    healthComp.maxHealth = 3;
    healthComp.resetHealth();


    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::TURRET, GameObjectType::TURRET_PLATFORM));
    
    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    return entityId;
}


int EntityManager::createChild(GameEngine& gameContext, Vector2 position, float rotation, GameObjectType goType) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    ColliderComponent& colliderComp = createComponent<ColliderComponent>(entityId);
    TriggerComponent& triggerComp = createComponent<TriggerComponent>(entityId);
    HealthComponent& healthComp = createComponent<HealthComponent>(entityId);
    RenderComponent& renderComp = createComponent<RenderComponent>(entityId);

    //######### DATA ########//
    situation.position = position;
    situation.rotation = rotation;
    situation.scale = { 0.18f, 0.18f };

    // Render component
    renderComp.sprite = "Media/Images/Personajes/Alien camuflado/Nino.png";
    renderComp.spriteRect = { 0, 116, 0, 274 };

    // Collider
    colliderComp.collisionLayer = ColliderComponent::Child;
    colliderComp.layerMasc = ColliderComponent::PlayerAttack + ColliderComponent::Player;
    colliderComp.type = ColliderType::STATIC;
    colliderComp.boundingRoot.bounding = { 0.f, 18.f, 0.f, 49.f };

    // Health
    healthComp.maxHealth = 1;
    healthComp.resetHealth();
    healthComp.score = -100;

    // Trigger
    triggerComp.functions.push_back(TriggerFunction::GIVE_CHILD_POINTS);


    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::CHILD, GameObjectType::CHILD));

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    return entityId;
}

int EntityManager::createFloatingText(GameEngine& gameContext, Vector2 position, float r, string text, Color color, bool isHUDElement, uint16_t size, float lifetime, float velocity) {
    int entityId = createText(gameContext, position, r, text, color, isHUDElement, size);

    VelocityComponent& velocityComp     = createComponent<VelocityComponent>(entityId);
    AutodeleteComponent& autodeleteComp = createComponent<AutodeleteComponent>(entityId);

    // Velocity
    velocityComp.gravity = 0.f;
    velocityComp.velocity.y = velocity;

    // Autodelete
    autodeleteComp.timeToDelete = lifetime;
    
    return entityId;
}

// ------------------------------ WORLD CREATION ------------------------------

int EntityManager::createWorld(GameEngine& gameContext, GameObjectType worldName) {
    int entityId = Entity::getCurrentId();

    WorldComponent& worldComp = createComponent<WorldComponent>(entityId);

    WorldElementsData::worldId = entityId;

    switch (worldName)
    {
    case GameObjectType::WORLD_DEBUG:
        worldComp.worldPath = "Media/Maps/debug.json";
        worldComp.backgroundSize = 0.75f;

        worldComp.backgroundLayers.clear(); // Delete previous layers
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/10_Sky.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/09_Forest.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/08_Forest.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/07_Forest.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/06_Forest.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/05_Particles.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/04_Forest.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/03_Particles.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/02_Bushes.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/01_Mist.png", { 255, 255, 255, 140 }));

        break;

    case GameObjectType::WORLD_1:
        worldComp.worldPath = "Media/Maps/level1.json";
        worldComp.backgroundSize = 0.75f;

        worldComp.backgroundLayers.clear();// Delete previous layers
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/10_Sky.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/09_Forest.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/08_Forest.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/07_Forest.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/06_Forest.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/05_Particles.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/04_Forest.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/03_Particles.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/02_Bushes.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/01_Mist.png", { 255, 255, 255, 140 }));

        break;

    case GameObjectType::WORLD_2:
        worldComp.worldPath = "Media/Maps/level2.json";
        worldComp.backgroundSize = 0.75f;

        worldComp.backgroundLayers.clear();// Delete previous layers
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/10_Sky.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/09_Forest.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/08_Forest.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/07_Forest.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/06_Forest.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/05_Particles.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/04_Forest.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/03_Particles.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/02_Bushes.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/01_Mist.png", { 255, 255, 255, 140 }));

        break;

    case GameObjectType::WORLD_3:
        worldComp.worldPath = "Media/Maps/level3.json";
        worldComp.backgroundSize = 0.75f;

        worldComp.backgroundLayers.clear();// Delete previous layers
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/10_Sky.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/09_Forest.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/08_Forest.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/07_Forest.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/06_Forest.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/05_Particles.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/04_Forest.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/03_Particles.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/02_Bushes.png"));
        worldComp.backgroundLayers.emplace_back(BackgroundLayer("Media/Backgrounds/Forest(Seamless)/01_Mist.png", { 255, 255, 255, 140 }));

        break;
    }

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::WORLD, worldName));

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().setBackgroundLayers(worldComp.backgroundLayers, worldComp.backgroundSize);

    
    // Create the entities of the world
    MapLoader::loadMapPhase(gameContext, worldComp.worldPath, "Phase1");
    worldComp.numberOfPhases = MapLoader::getNumberOfPhases(worldComp.worldPath);

    return entityId;
}


// ------------------------------ MENUS CREATION ------------------------------

int EntityManager::createMenu(GameEngine& gameContext, GameObjectType menuType) {
    int entityId = Entity::getCurrentId();

    MenuComponent& menuComp = createComponent<MenuComponent>(entityId);
    RenderComponent& renderComp = createComponent<RenderComponent>(entityId);
    SituationComponent& sitComp = createComponent<SituationComponent>(entityId);

    //Situation
    sitComp.position = { 0.f, 0.f };
    sitComp.noWorldDelete = true;

    // Render
    renderComp.color = {255, 255, 255, 255};
    renderComp.isHUDElement = true;
    renderComp.spriteRect = { 0, 800, 0, 600 };

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::MENU, menuType));


    // After creating the menu entity, create the menu Options
    if (menuType == GameObjectType::PAUSE) {
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(280.f, 260.f), 0.f, MenuOptions::PLAY));
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(280.f, 350.f), 0.f, MenuOptions::CONTROLS));
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(280.f, 440.f), 0.f, MenuOptions::EXIT));

        renderComp.sprite = "Media/Images/Menu/Pause.png";

    }
    else if (menuType == GameObjectType::CONTROLS_KEYBOARD) {
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(280.f, 220.f), 0.f, MenuOptions::SET_KEY_ATTACK));
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(280.f, 250.f), 0.f, MenuOptions::SET_KEY_JUMP));
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(280.f, 280.f), 0.f, MenuOptions::SET_KEY_LEFT));
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(280.f, 310.f), 0.f, MenuOptions::SET_KEY_RIGHT));
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(280.f, 340.f), 0.f, MenuOptions::SET_KEY_UP));
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(280.f, 370.f), 0.f, MenuOptions::SET_KEY_DOWN));
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(280.f, 450.f), 0.f, MenuOptions::BACK));

        Utils::setControlKeyToMenuOptions(gameContext, menuComp);

        renderComp.sprite = "Media/Images/Menu/ControlsKeyboard.png";
    }
    else if (menuType == GameObjectType::CONTROLS_JOYSTICK) {
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(280.f, 220.f), 0.f, MenuOptions::SET_KEY_ATTACK));
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(280.f, 250.f), 0.f, MenuOptions::SET_KEY_JUMP));
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(280.f, 280.f), 0.f, MenuOptions::SET_KEY_LEFT));
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(280.f, 310.f), 0.f, MenuOptions::SET_KEY_RIGHT));
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(280.f, 340.f), 0.f, MenuOptions::SET_KEY_UP));
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(280.f, 370.f), 0.f, MenuOptions::SET_KEY_DOWN));

        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(500.f, 220.f), 0.f, MenuOptions::SET_JOYSTICK_ATTACK));
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(500.f, 250.f), 0.f, MenuOptions::SET_JOYSTICK_JUMP));

        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(280.f, 450.f), 0.f, MenuOptions::BACK));

        Utils::setControlKeyToMenuOptions(gameContext, menuComp);

        renderComp.sprite = "Media/Images/Menu/ControlsJoystick.png";
    }
    else if (menuType == GameObjectType::BEST_SCORES) {
        gameContext.entityMan->readBestScore();

        // Create the text of the best scores
        auto& best_score_list = WorldElementsData::best_score_list;
        Vector2 pos = { 230.f, 80.f };

        for (std::size_t i = 0; i < best_score_list.size(); ++i) {
            std::string text = best_score_list[i].name + " __________________ " + to_string(best_score_list[i].score);
            pos.y += 40.f;
            menuComp.textsId.emplace_back(createText(gameContext, pos, 0.f, text, { 239, 184, 4, 255 }, true, 20));
        }

        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(280.f, 530.f), 0.f, MenuOptions::MAIN_MENU));

        renderComp.sprite = "Media/Images/Menu/highScores.png";
    }
    else if( menuType == GameObjectType::GAME_OVER_MENU){
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(280.f, 400.f), 0.f, MenuOptions::NEW_BEST_SCORE));

        renderComp.sprite = "Media/Images/Menu/GameOver.png";
    }
    else if(menuType == GameObjectType::NEW_BEST_SCORE) {
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(180.f, 306.f), 0.f, MenuOptions::SELECT_CHARACTER));
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(220.f, 306.f), 0.f, MenuOptions::SELECT_CHARACTER));
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(260.f, 306.f), 0.f, MenuOptions::SELECT_CHARACTER));

        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(350.f, 306.f), 0.f, MenuOptions::TO_BEST_SCORES, "ADD NEW SCORE"));

        renderComp.sprite = "Media/Images/Menu/NewHighscore.png";
    }
    else if (menuType == GameObjectType::MAINMENU) {
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(30.f, 350.f), 0.f, MenuOptions::PLAY));
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(30.f, 440.f), 0.f, MenuOptions::TO_BEST_SCORES, "BEST SCORES"));
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(30.f, 530.f), 0.f, MenuOptions::EXIT));

        renderComp.sprite = "Media/PNG/Animacion menu/animationTilesheet.png";

        // Animation
        AnimationComponent& animComp = createComponent<AnimationComponent>(entityId);
        AnimationManager::setAnimationToEntity(gameContext, Animation::IDLE, animComp);
        // Music
        menuComp.menuMusic.soundPath = "./Media/Sound/Music/arthur-vyncke-a-few-jumps-away.wav";
        menuComp.menuMusic.repeat = true;
    }
    else if (menuType == GameObjectType::NEXT_LEVEL_MENU) {
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(280.f, 400.f), 0.f, MenuOptions::TO_NEXT_LEVEL));

        renderComp.sprite = "Media/Images/Menu/NextLevel.png";
    }
    else if (menuType == GameObjectType::WIN_MENU) {
        menuComp.optionsId.emplace_back(createMenuOption(gameContext, Vector2(280.f, 510.f), 0.f, MenuOptions::NEW_BEST_SCORE));

        renderComp.sprite = "Media/Images/Menu/Win.png";
    }


    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    return entityId;
}


int EntityManager::createMenuOption(GameEngine& gameContext, Vector2 position, float r, MenuOptions menuOpt, string text) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation   = createComponent<SituationComponent>(entityId);
    MenuOptionComponent& optionComp = createComponent<MenuOptionComponent>(entityId);
    //RenderComponent& renderComp = createComponent<RenderComponent>(entityId);
    TextComponent& textComp = createComponent<TextComponent>(entityId);

    //######### DATA ########//
    situation.position = position;
    situation.rotation = r;
    situation.noWorldDelete = true;

    optionComp.option = menuOpt;
    optionComp.acceptSound.soundPath = "./Media/Sound/UI/confirm_style_4_001.wav";

    textComp.isHUDElement = true;
    textComp.size = 40;

    switch (menuOpt)
    {
    case MenuOptions::PLAY:
        textComp.text = "PLAY";

        break;

    case MenuOptions::EXIT:
        textComp.text = "EXIT";

        break;

    case MenuOptions::BACK:
        textComp.text = "BACK";
        optionComp.acceptSound.soundPath = "./Media/Sound/UI/back_style_4_001.wav";

        break;

    case MenuOptions::CONTROLS:
        textComp.text = "CONTROLS";

        break;

    case MenuOptions::MAIN_MENU:
        textComp.text = "MAIN MENU";

        break;

    case MenuOptions::TO_NEXT_LEVEL:
        textComp.text = "LET'S GO!";

        break;

    case MenuOptions::TO_BEST_SCORES:
        if (text != "") {
            textComp.text = text;
        }
        else {
            textComp.text = "BEST SCORES";
        }

        break;

    case MenuOptions::NEW_BEST_SCORE:
        textComp.text = "BEST SCORES";

        break;

    case MenuOptions::SELECT_CHARACTER:
        textComp.text = "A";

        break;


    // set controls, keys
    case MenuOptions::SET_KEY_ATTACK:
        textComp.text = "ATTACK: ";
        textComp.size = 20;
        break;

    case MenuOptions::SET_KEY_JUMP:
        textComp.text = "JUMP: ";
        textComp.size = 20;
        break;

    case MenuOptions::SET_KEY_LEFT:
        textComp.text = "LEFT: ";
        textComp.size = 20;
        break;

    case MenuOptions::SET_KEY_RIGHT:
        textComp.text = "RIGHT: ";
        textComp.size = 20;
        break;

    case MenuOptions::SET_KEY_UP:
        textComp.text = "UP: ";
        textComp.size = 20;
        break;

    case MenuOptions::SET_KEY_DOWN:
        textComp.text = "DOWN: ";
        textComp.size = 20;
        break;


    // set controls, Joystick
    case MenuOptions::SET_JOYSTICK_ATTACK:
        textComp.text = "ATTACK: ";
        textComp.size = 20;
        break;

    case MenuOptions::SET_JOYSTICK_JUMP:
        textComp.text = "JUMP: ";
        textComp.size = 20;
        break;
    }

    gameContext.getWindowFacadeRef().createText(gameContext, entityId);

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::MENU_OPTION, GameObjectType::NONE));
    return entityId;
}


// ------------------------------ HUD CREATION --------------------------------

int EntityManager::createHUDElement(GameEngine& gameContext, Vector2 position, float r, GameObjectType objType) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation = createComponent<SituationComponent>(entityId);

    //######### DATA ########//
    situation.position = position;
    situation.rotation = r;
    situation.noWorldDelete = true;

    // Render component
    /*renderComp.sprite = "Media/Images/YellowDuck.png";
    renderComp.spriteRect = { 4, 34, 4, 34 };
    renderComp.isHUDElement = true;
    renderComp.color = { 255, 0, 0, 255 };*/

    if (objType == GameObjectType::HUD_PLAYER_HEALTH) {
        TextComponent& textComp = createComponent<TextComponent>(entityId);

        textComp.color = { 255, 20, 30, 255 };
        textComp.isHUDElement = true;
        textComp.size = 30;
        textComp.text = "";
    }
    else if (objType == GameObjectType::HUD_PLAYER_AMMO) {
        TextComponent& textComp = createComponent<TextComponent>(entityId);

        textComp.color = { 0, 0, 0, 255 };
        textComp.isHUDElement = true;
        textComp.size = 24;
        textComp.text = "";
    }
    else if (objType == GameObjectType::HUD_PLAYER_LIFES) {
        TextComponent& textComp = createComponent<TextComponent>(entityId);

        textComp.color = { 0, 0, 0, 255 };
        textComp.isHUDElement = true;
        textComp.size = 24;
        textComp.text = "";
    }
    else if (objType == GameObjectType::HUD_PLAYER_SCORE) {
        TextComponent& textComp = createComponent<TextComponent>(entityId);

        textComp.color = { 249, 207, 19, 255 };
        textComp.isHUDElement = true;
        textComp.size = 30;
        textComp.text = "";
    }

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::HUD_ELEMENT, objType));

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createText(gameContext, entityId);

    return entityId;
}


int EntityManager::createText(GameEngine& gameContext, Vector2 position, float r, string text, Color color, bool isHUDElement, uint16_t size) {
    int entityId = Entity::getCurrentId();

    SituationComponent& situation = createComponent<SituationComponent>(entityId);
    TextComponent& textComp = createComponent<TextComponent>(entityId);

    //######### DATA ########//
    situation.position = position;
    situation.rotation = r;

    //if(isHUDElement)
        situation.noWorldDelete = true;

    textComp.color = color;
    textComp.isHUDElement = isHUDElement;
    textComp.size = size;
    textComp.text = text;
       

    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(EntityType::TEXT, GameObjectType::TEXT));


    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createText(gameContext, entityId);
    return entityId;
}




void EntityManager::readBestScore() {
    auto& best_score_list = WorldElementsData::best_score_list;

    // Input (Read Json)
    std::ifstream file("score.json");
    json jData = json::parse(file);

    for (std::size_t i = 0; i < best_score_list.size(); ++i) {
        best_score_list[i].name = jData["best_score_list"][i]["name"].get<string>();
        best_score_list[i].score = jData["best_score_list"][i]["score"].get<int>();
    }


    /*for (std::size_t i = 0; i < best_score_list.size(); ++i) {
        std::cout << "Name: " << best_score_list[i].name << " score: " << best_score_list[i].score << "\n";
    }*/
}

void EntityManager::writeBestScore() {
    auto& best_score_list = WorldElementsData::best_score_list;
    json jData;

    for (std::size_t i = 0; i < best_score_list.size(); ++i) {
         jData["best_score_list"][i]["name"] = best_score_list[i].name;
         jData["best_score_list"][i]["score"] = best_score_list[i].score;
    }

    std::ofstream file("score.json");
    file << jData;
}