#pragma once

#include <unordered_map>
class GameEngine;

#include <tools/Storage.hpp>
#include <tools/Vector2.hpp>
#include <com/ComponentsIncluder>
#include <enum/EnumsIncluder>
#include <ent/Entity.hpp>


class EntityManager {
public:

    //////////////////////////////////////////////////
    //              THE RULE OF THE 3               //
    //////////////////////////////////////////////////

    explicit EntityManager();
    ~EntityManager();
    EntityManager(const EntityManager &em) = delete;

    //////////////////////////////////////////////////
    //                  GET COMPONENT               //
    //////////////////////////////////////////////////

    template <typename T>
    T  &getComponent(const int id) {
        auto &storage = getComponents<T>();
        //return storage.at(storage.at(id));
        return storage[storage[id]];
    }

    template <typename T>
    Storage<T>  &getComponents() {
        static Storage<T> storage{MAX_ENTITIES};
        return storage;
    }

    template <typename T>
    bool existsComponent(const int id) {
        auto &storage = getComponents<T>();
        return storage.exists(id);
    }


    //////////////////////////////////////////////////
    //                CREATE COMPONENT              //
    //////////////////////////////////////////////////

    template <typename T>
    T &createComponent(const int id) {
        auto &storage = getComponents<T>();
        storage.emplace(id);
        //return storage.at(storage.at(id));
        return storage[storage[id]];
    }


    //////////////////////////////////////////////////
    //                ERASE COMPONENT               //
    //////////////////////////////////////////////////

    template <typename T>
    void eraseComponent(const int id) {
        auto &storage = getComponents<T>();
        storage.erase(id);
    }


    //////////////////////////////////////////////////
    //                GET ENTITY                    //
    //////////////////////////////////////////////////
    Entity &getEntity(int id);
    std::unordered_map<int, Entity> &getEntities();
    bool existEntity(int id) const;


    //////////////////////////////////////////////////
    //                ERASE ENTITY                  //
    //////////////////////////////////////////////////
    void eraseEntityByID                (int id);


    //////////////////////////////////////////////////
    //                UPDATE ENTITY                 //
    //////////////////////////////////////////////////
    const std::vector<int>   &getEntitiesToUpdate();
    void addEntityToUpdate(const int id);
    void removeEntityToUpdate(const int id);
    void clearEntitiesToUpdate();

    //////////////////////////////////////////////////
    //                ENTITY CREATION               //
    //////////////////////////////////////////////////
    int createPlayer(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType = GameObjectType::NONE);
    int createPlayerHand(GameEngine& gameContext, int playerId);
    int createAttack(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType = GameObjectType::NONE);
    int createWall(GameEngine& gameContext, Vector2 position, Vector2 size, float r, GameObjectType goType = GameObjectType::NONE);
    int createEnemy(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType = GameObjectType::NONE);
    int createDrone(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType);
    int createWeapon(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType = GameObjectType::NONE);
    int createCamera(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType = GameObjectType::NONE);
    int createBomb(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType = GameObjectType::NONE);
    int createSpawner(GameEngine& gameContext, Vector2 position, float r, GameObjectType goTypeToSpawn = GameObjectType::NONE);
    int createShield(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType = GameObjectType::NONE);
    int createPowerUp(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType = GameObjectType::NONE);
    int createTrigger(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType = GameObjectType::NONE);
    int createDamagePlatform(GameEngine& gameContext, Vector2 position, Vector2 size, uint16_t damage, GameObjectType goType);
    int createOrbitalMarker(GameEngine& gameContext, Vector2 position, GameObjectType goType = GameObjectType::NONE);
    int createOrbitalStrikerEnemy(GameEngine& gameContext, Vector2 position, GameObjectType goType);
    int createChild(GameEngine& gameContext, Vector2 position, float rotation, GameObjectType goType);
    int createFloatingText(GameEngine& gameContext, Vector2 position, float r, string text, Color color, bool isHUDElement, uint16_t size, float lifetime, float velocity);

    void createTurret(GameEngine& gameContext, Vector2 position, uint8_t facing);
    int createTurretPlatform(GameEngine& gameContext, Vector2 position, uint8_t facing, int turretGun, int turretText, GameObjectType goType = GameObjectType::NONE);
    int createTurretGun(GameEngine& gameContext, Vector2 position, uint8_t facing, GameObjectType goType = GameObjectType::NONE);



    //////////////////////////////////////////////////
    //                WORLD CREATION               //
    //////////////////////////////////////////////////
    int createWorld(GameEngine& gameContext, GameObjectType worldName);

    //////////////////////////////////////////////////
    //                MENU CREATION               //
    //////////////////////////////////////////////////
    int createMenu(GameEngine& gameContext, GameObjectType menuType);
    int createMenuOption(GameEngine& gameContext, Vector2 position, float r, MenuOptions menuOpt, string text = "");


    //////////////////////////////////////////////////
    //                HUD CREATION                  //
    //////////////////////////////////////////////////
    int createHUDElement(GameEngine& gameContext, Vector2 position, float r, GameObjectType menuType);
    int createText(GameEngine& gameContext, Vector2 position, float r, string text, Color color, bool isHUDElement, uint16_t size);





    void readBestScore();
    void writeBestScore();
private:

    std::unordered_map<int, Entity> entityMap;
    std::vector<int> entitiesToUpdate;
    const uint32_t MAX_ENTITIES = 9092;

};
