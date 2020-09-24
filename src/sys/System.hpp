#pragma once

class GameEngine;
#include <ent/Entity.hpp>
#include <com/ComponentsForwardDeclaration>
#include <enum/EnumsIncluder>
#include <stdint.h>
#include <vector>
#include <tools/WorldData.hpp>

class System {
public:
    virtual ~System() = default;
    virtual void update(GameEngine &gameContext) const = 0;

private:

};

