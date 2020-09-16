#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class CollisionSystem : public System {
public:
    CollisionSystem();
    ~CollisionSystem();

    void update(GameEngine& gameContext) const override;

private:
    void calculateCollisions(GameEngine& gameContext) const;
    void insertCollidersIdWithVelocity(GameEngine& gameContext, std::vector<int>& idCollidersWithVelocity) const;
    bool checkCollisionAB(BoundingBoxNode& boundingA, SituationComponent& situationA, BoundingBoxNode& boundingB, SituationComponent& situationB) const;
    void undoCollision(GameEngine& gameContext, ColliderComponent& solidCol, ColliderComponent& mobileCol) const;
    void clearCollisions(ColliderComponent& collider) const;
};