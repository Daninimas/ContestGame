#pragma once

#include <unordered_map>
class GameEngine;

#include <tools/Storage.hpp>
#include <com/ComponentsIncluder>
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
    //                ENTITY CREATION               //
    //////////////////////////////////////////////////

    //int createEntity           (GameEngine &gameContext, EntityType eType, float x, float y, float z, GameObjectType goType = NONE);


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
    std::vector<int>   &getEntitiesToUpdate();
    void addEntityToUpdate(const int id);
    void clearEntitiesToUpdate();

private:

    std::unordered_map<int, Entity> entityMap;
    std::vector<int> entitiesToUpdate;
    const uint32_t MAX_ENTITIES = 9092;

};
