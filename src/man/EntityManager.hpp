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
    int createAttack(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType = GameObjectType::NONE);
    int createWall(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType = GameObjectType::NONE);
    int createEnemy(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType = GameObjectType::NONE);
    int createWeapon(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType = GameObjectType::NONE);
    int createCamera(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType = GameObjectType::NONE);
    int createBomb(GameEngine& gameContext, Vector2 position, float r, GameObjectType goType = GameObjectType::NONE);
    int createSpawner(GameEngine& gameContext, Vector2 position, float r, GameObjectType goTypeToSpawn = GameObjectType::NONE);
    int createShield(GameEngine& gameContext, Vector2 position, float r, GameObjectType goTypeToSpawn = GameObjectType::NONE);
    int createPowerUp(GameEngine& gameContext, Vector2 position, float r, GameObjectType goTypeToSpawn = GameObjectType::NONE);


    //////////////////////////////////////////////////
    //                WORLD CREATION               //
    //////////////////////////////////////////////////
    void createWorld(GameEngine& gameContext, WorldEnum worldName);

    //////////////////////////////////////////////////
    //                MENU CREATION               //
    //////////////////////////////////////////////////
    int createMenu(GameEngine& gameContext, GameObjectType menuType);

private:

    std::unordered_map<int, Entity> entityMap;
    std::vector<int> entitiesToUpdate;
    const uint32_t MAX_ENTITIES = 9092;

};
