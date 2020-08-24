#pragma once

#include <enum/EntityType.hpp>
#include <enum/GameObjectType.hpp>

class Entity {
public:
    Entity(EntityType t, GameObjectType goT = GameObjectType::NONE);
    // Entity(Entity& e) = delete;
    ~Entity();

    const int getId() const;
    EntityType getType() const;
    GameObjectType getGameObjectType() const;
    static int getCurrentId();

private:
    int id;
    EntityType type;
    GameObjectType goType;
};