#include "Entity.hpp"
#include <limits>

static int currentId = std::numeric_limits<int>::min();

Entity::Entity()
    : id {currentId++} {}

Entity::~Entity() {}

const int Entity::getId() const {
    return id;
}

int Entity::getCurrentId() {
    return currentId;
}