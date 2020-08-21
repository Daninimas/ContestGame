#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class StaticEntitiesSystem : public System {
public:
    StaticEntitiesSystem();
    ~StaticEntitiesSystem();

    void 	init(GameEngine &gameContext) const;

private:
    void    createPlayer(GameEngine &gameContext) const;
};