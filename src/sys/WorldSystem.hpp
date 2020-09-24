#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class WorldSystem : public System {
public:
    WorldSystem();
    ~WorldSystem();

    void update(GameEngine& gameContext) const override;

private:
    void deleteEntitiesOutOfWorld(GameEngine& gameContext) const;
};