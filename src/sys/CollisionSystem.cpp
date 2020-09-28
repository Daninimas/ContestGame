#include "CollisionSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/BoundingBox.hpp>
#include <tools/Utils.hpp>
#include <iostream>
#include <functional>

/*
 ###### OPTIMIZATION V.2 #######
   Calculate collision entity with (velocity-all collidables)
   Collision calculate when they can collide with the layerMasc
*/

CollisionSystem::CollisionSystem() {}

CollisionSystem::~CollisionSystem() {}


void CollisionSystem::update(GameEngine& gameContext) const {
    clearAllCollisions(gameContext);

	calculateCollisions(gameContext);
}


void CollisionSystem::clearAllCollisions(GameEngine& gameContext) const {
    auto& colliders = gameContext.entityMan.getComponents<ColliderComponent>();

    for (auto& collider : colliders) {
        clearCollisions(collider);
    }
}


void CollisionSystem::calculateCollisions(GameEngine& gameContext) const {
    std::size_t velocitiesSize = gameContext.entityMan.getComponents<VelocityComponent>().size();
    auto&       colliders      = gameContext.entityMan.getComponents<ColliderComponent>();
    std::vector<int> idCollidersWithVelocity;

    // Prepare data for the collision calculation
    idCollidersWithVelocity.reserve(velocitiesSize); // Reserve the size of the velocities component storage
    Utils::insertCollidersIdWithVelocity(gameContext, idCollidersWithVelocity);

    // Calculate collision
    for (std::size_t i = 0; i < idCollidersWithVelocity.size(); ++i) {
        int idA = idCollidersWithVelocity[i];
        ColliderComponent& colliderA = gameContext.entityMan.getComponent<ColliderComponent>(idA);
        SituationComponent& situationA = gameContext.entityMan.getComponent<SituationComponent>(idA);

        for (auto& colliderB : colliders) {
            int idB = colliderB.id;

            if (idA != idB && ( colliderA.layerMasc & colliderB.collisionLayer && colliderB.layerMasc & colliderA.collisionLayer) ) {
                SituationComponent& situationB = gameContext.entityMan.getComponent<SituationComponent>(idB);
                
                bool collided = checkCollisionAB(colliderA.boundingRoot, situationA, colliderB.boundingRoot, situationB);

                if (collided) {
                    colliderA.collide = true;
                    colliderB.collide = true;

                    if (!(colliderA.type == ColliderType::NO_SOLID) && !(colliderB.type == ColliderType::NO_SOLID)) {
                        // Check who is the static and the dinamic
                        if (colliderA.type == ColliderType::DYNAMIC) {
                            undoCollision(gameContext, colliderB, colliderA);
                        }
                        else if (colliderB.type == ColliderType::DYNAMIC) {
                            undoCollision(gameContext, colliderA, colliderB);
                        }
                        // No one is dynamic, don't need to resolve
                    }

                }

            }
        }
    }
}


bool CollisionSystem::checkCollisionAB(BoundingBoxNode& boundingA, SituationComponent& situationA, BoundingBoxNode& boundingB, SituationComponent& situationB) const {
    bool collide = false;
    BoundingBox bA = Utils::moveToWorldCoords(boundingA.bounding, situationA);
    BoundingBox bB = Utils::moveToWorldCoords(boundingB.bounding, situationB);

    auto checkIntervals = [](float L1, float R1, float L2, float R2) {
        if (L2 >= R1) return false;
        if (L1 >= R2) return false;
        return true;
    };

    if(    checkIntervals(bA.xLeft, bA.xRight, bB.xLeft, bB.xRight)
        && checkIntervals(bA.yUp, bA.yDown, bB.yUp, bB.yDown) ) {
        // They are colliding
        if (boundingA.childs.size() > 0) {
            for (auto& b : boundingA.childs) {
                 collide = checkCollisionAB(b, situationA, boundingB, situationB);
            }
        }
        else if(boundingB.childs.size() > 0){
            for (auto& b : boundingB.childs) {
                collide = checkCollisionAB(boundingA, situationA, b, situationB);
            }
        }

        if(!collide) {
            // TODO si no uso en el futuro para nada los entitiesCollifind IDs, borrarlos y usar un BOOL (OPTIMIZACION)

            //comprobar que no lo meta dos veces
            if (std::find(boundingA.bounding.entitiesColliding.begin(), boundingA.bounding.entitiesColliding.end(), situationB.id) == boundingA.bounding.entitiesColliding.end()) {
                // someName not in name, add it
                boundingA.bounding.entitiesColliding.push_back(situationB.id);
            }
            if (std::find(boundingB.bounding.entitiesColliding.begin(), boundingB.bounding.entitiesColliding.end(), situationA.id) == boundingB.bounding.entitiesColliding.end()) {
                // someName not in name, add it
                boundingB.bounding.entitiesColliding.push_back(situationA.id);
            }

            collide = true;
            
        }
    }

    return collide;
}

void CollisionSystem::undoCollision(GameEngine& gameContext, ColliderComponent& solidCol, ColliderComponent&  mobileCol) const {
    VelocityComponent& mobileVel  = gameContext.entityMan.getComponent<VelocityComponent>(mobileCol.id);
    SituationComponent& sitSolid  = gameContext.entityMan.getComponent<SituationComponent>(solidCol.id);
    SituationComponent& sitMobile = gameContext.entityMan.getComponent<SituationComponent>(mobileCol.id);
    // This will change the situation of the mobile component to resolve the collision

    BoundingBox solidBounding  = Utils::moveToWorldCoords(solidCol.boundingRoot.bounding, sitSolid);
    BoundingBox mobileBounding = Utils::moveToWorldCoords(mobileCol.boundingRoot.bounding, sitMobile);

    auto calculateIntersection = [](float mobLeft, float mobRight, float solLeft, float solRight) -> float{ // trailing return type
        if (mobLeft < solLeft) {
            if (mobRight < solRight) {
                // Left
                return solLeft - mobRight;
            }
        }
        else if (mobRight > solRight) {
            return solRight - mobLeft;
        }

        return 0.f;
    };

    float overlapX = calculateIntersection(mobileBounding.xLeft, mobileBounding.xRight, solidBounding.xLeft, solidBounding.xRight);
    float overlapY = calculateIntersection(mobileBounding.yUp, mobileBounding.yDown, solidBounding.yUp, solidBounding.yDown);
   

    if ( overlapX == 0 || (overlapY != 0 && std::abs(overlapY) <= std::abs(overlapX)) ) {
        sitMobile.y += overlapY;
        mobileVel.velocityY = 0.f;
    }
    else {
        sitMobile.x += overlapX;
        mobileVel.velocityX = 0.f;
    }

    gameContext.entityMan.addEntityToUpdate(mobileCol.id);
}


void CollisionSystem::clearCollisions(ColliderComponent& collider) const {
    collider.collide = false;

    std::function<void(BoundingBoxNode&)> clearCollidersID = [&](BoundingBoxNode& b) {
        b.bounding.entitiesColliding.clear();
        for (auto& c : b.childs) {
            clearCollidersID(c);
        }
    };

    clearCollidersID(collider.boundingRoot);
}