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
    void checkPhaseCollision(GameEngine& gameContext) const;

    void collideWithPhaseBounding(GameEngine& gameContext, int entityId, BoundingBox& entityBounding) const;

    void moveBackgroundLayers(GameEngine& gameContext) const;

    void checkScoreSound(GameEngine& gameContext) const;
};