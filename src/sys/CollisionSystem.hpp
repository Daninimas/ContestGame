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
    bool checkCollisionAB(GameEngine& gameContext, ColliderComponent& colliderA, SituationComponent& situationA, ColliderComponent& colliderB, SituationComponent& situationB) const;
};