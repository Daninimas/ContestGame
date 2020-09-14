#include "CollisionSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/BoundingBox.hpp>
#include <tools/Utils.hpp>
#include <iostream>
#include <functional>

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

            // clear collisions
            clearCollisions(colliderA);
            clearCollisions(colliderB);

            if (idA == idB) {
                // Do nothing. Ask Fran (FOR OPTIMIZATION)
            }
            else {
                SituationComponent& situationB = gameContext.entityMan.getComponent<SituationComponent>(idB);
                
                checkCollisionAB(colliderA.boundingRoot, situationA, colliderB.boundingRoot, situationB);
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


bool CollisionSystem::checkCollisionAB(BoundingBoxNode& boundingA, SituationComponent& situationA, BoundingBoxNode& boundingB, SituationComponent& situationB) const {
    BoundingBox bA = Utils::moveToWorldCoords(boundingA.bounding, situationA);
    BoundingBox bB = Utils::moveToWorldCoords(boundingB.bounding, situationB);

    auto checkIntervals = [](float L1, float R1, float L2, float R2) {
        if (L2 > R1) return false;
        if (L1 > R2) return false;
        return true;
    };

    if(    checkIntervals(bA.xLeft, bA.xRight, bB.xLeft, bB.xRight)
        && checkIntervals(bA.yUp, bA.yDown, bB.yUp, bB.yDown) ) {
        cout << "Colisionaaaa\n";

        // They are colliding
        if (boundingA.childs.size() > 0) {
            for (auto& b : boundingA.childs) {
                checkCollisionAB(b, situationA, boundingB, situationB);
            }
        }
        else if(boundingB.childs.size() > 0){
            for (auto& b : boundingB.childs) {
                checkCollisionAB(boundingA, situationA, b, situationB);
            }
        }
        else {
            // TODO si no uso en el futuro para nada los entitiesCollifind IDs, borrarlos y usar un BOOL (OPTIMIZACION)
            boundingA.bounding.entitiesColliding.push_back(situationB.id);
            boundingB.bounding.entitiesColliding.push_back(situationA.id);

            cout << "Colisionaaaa con la cabeza\n";
        }
    }
    return false;
}


void CollisionSystem::clearCollisions(ColliderComponent& collider) const {
    
    std::function<void(BoundingBoxNode&)> clearCollidersID = [&](BoundingBoxNode& b) {
        b.bounding.entitiesColliding.clear();
        for (auto& c : b.childs) {
            clearCollidersID(c);
        }
    };

    clearCollidersID(collider.boundingRoot);
}