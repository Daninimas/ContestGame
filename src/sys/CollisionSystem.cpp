#include "CollisionSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/BoundingBox.hpp>
#include <tools/Utils.hpp>
#include <iostream>

/*
 ###### OPTIMIZATION V.1 #######
   Calculate collision entity with (velocity-all collidables)
*/

CollisionSystem::CollisionSystem() {}

CollisionSystem::~CollisionSystem() {}


void CollisionSystem::update(GameEngine& gameContext) const {
	calculateCollisions(gameContext);
}


void CollisionSystem::calculateCollisions(GameEngine& gameContext) const {
    std::size_t velocitiesSize = gameContext.entityMan.getComponents<VelocityComponent>().size();
    auto&       colliders      = gameContext.entityMan.getComponents<ColliderComponent>();
    std::vector<int> idCollidersWithVelocity;

    // Prepare data for the collision calculation
    idCollidersWithVelocity.reserve(velocitiesSize); // Reserve the size of the velocities component storage
    insertCollidersIdWithVelocity(gameContext, idCollidersWithVelocity);


    // Calculate collision
    for (std::size_t i = 0; i < idCollidersWithVelocity.size(); ++i) {
        int idA = idCollidersWithVelocity[i];
        ColliderComponent& colliderA = gameContext.entityMan.getComponent<ColliderComponent>(idA);
        SituationComponent& situationA = gameContext.entityMan.getComponent<SituationComponent>(idA);

        for (auto& colliderB : colliders) {
            int idB = colliderB.id;

            if (idA == idB) {
                // Do nothing. Ask Fran (FOR OPTIMIZATION)
            }
            else {
                SituationComponent& situationB = gameContext.entityMan.getComponent<SituationComponent>(idB);
                
                checkCollisionAB(gameContext, colliderA, situationA, colliderB, situationB);
            }
        }
    }
}

void CollisionSystem::insertCollidersIdWithVelocity(GameEngine& gameContext, std::vector<int>& idCollidersWithVelocity) const {
    auto& velocitiesComps = gameContext.entityMan.getComponents<VelocityComponent>();
    auto& colliders = gameContext.entityMan.getComponents<ColliderComponent>();
    for (auto& velComp : velocitiesComps) {
        int idVelEnt = velComp.id;

        if (colliders.exists(idVelEnt)) {
            // has the two components
            idCollidersWithVelocity.emplace_back(idVelEnt);
        }
    }
}


bool CollisionSystem::checkCollisionAB(GameEngine& gameContext, ColliderComponent& colliderA, SituationComponent& situationA, ColliderComponent& colliderB, SituationComponent& situationB) const {
    BoundingBox boundingA = Utils::moveToWorldCoords(colliderA.boundingRoot.bounding, situationA);
    BoundingBox boundingB = Utils::moveToWorldCoords(colliderB.boundingRoot.bounding, situationB);

    auto checkIntervals = [](float L1, float R1, float L2, float R2) {
        if (L2 > R1) return false;
        if (L1 > R2) return false;
        return true;
    };

    if(    checkIntervals(boundingA.xLeft, boundingA.xRight, boundingB.xLeft, boundingB.xRight)
        && checkIntervals(boundingA.yUp, boundingA.yDown, boundingB.yUp, boundingB.yDown) ) {
        // They are colliding
    }
    return false;
}