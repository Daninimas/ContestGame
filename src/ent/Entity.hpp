#pragma once

#include <stdint.h>

class Entity {
public:
    Entity();
   // Entity(Entity& e) = delete;
    ~Entity();

    const int getId() const;
    static int getCurrentId();

private:
    int id;
};