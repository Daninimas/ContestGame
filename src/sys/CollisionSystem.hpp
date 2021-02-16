#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class CollisionSystem : public System {
public:
    CollisionSystem();
    ~CollisionSystem();

    void update(GameEngine& gameContext) const override;

private:
    void clearAllCollisions(GameEngine& gameContext) const;
    void calculateCollisions(GameEngine& gameContext) const;
    bool checkCollisionAB(BoundingBoxNode& boundingA, SituationComponent& situationA, BoundingBoxNode& boundingB, SituationComponent& situationB) const;
    void undoCollision(GameEngine& gameContext, ColliderComponent& solidCol, ColliderComponent& mobileCol) const;
    void clearCollisions(ColliderComponent& collider) const;

    bool checkCollisionWithPlatform(GameEngine& gameContext, ColliderComponent& platformColl, ColliderComponent& entityColl, SituationComponent& platformSit, SituationComponent& entitySit) const;
};