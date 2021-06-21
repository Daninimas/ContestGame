#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>
#include <array>

class StaticEntitiesSystem : public System {
public:
    StaticEntitiesSystem();
    ~StaticEntitiesSystem();

    void 	init(GameEngine &gameContext) const;
    void 	update(GameEngine& gameContext) const override;

private:
    void    createPlayer(GameEngine& gameContext) const;
    void    createWorld(GameEngine &gameContext) const;


    std::array<GameObjectType, 2> worldGameObjets {GameObjectType::WORLD_DEBUG, GameObjectType::WORLD_1};
};