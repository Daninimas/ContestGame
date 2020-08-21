#include "Entity.hpp"
#include <limits>

static int currentId = std::numeric_limits<int>::min();

Entity::Entity(EntityType t, GameObjectType goT) :
    id{ currentId++ }, type{ t }, goType{ goT } {}

Entity::~Entity() {}

const int Entity::getId() const {
    return id;
}


EntityType Entity::getType() const {
    return type;
}

GameObjectType Entity::getGameObjectType() const {
    return goType;
}

int Entity::getCurrentId() {
    return currentId;
}