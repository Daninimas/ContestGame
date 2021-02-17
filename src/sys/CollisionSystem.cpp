#include "CollisionSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/BoundingBox.hpp>
#include <tools/Utils.hpp>
#include <iostream>
#include <functional>

/*
 ###### OPTIMIZATION V.3 #######
   Calculate collision entity with (not wall-all collidables)
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
    auto& colliders = gameContext.entityMan.getComponents<ColliderComponent>();
    std::vector<std::reference_wrapper<ColliderComponent>> collidersNotWall; // vector of references to colliders

    // Prepare data for the collision calculation
    collidersNotWall.reserve(colliders.size()); // Reserve the size of the velocities component storage
    //Utils::insertCollidersIdWithVelocity(gameContext, collidersNotWall);
    Utils::insertNotWallColliders(gameContext, collidersNotWall);

    // Calculate collision
    for (ColliderComponent& colliderA : collidersNotWall) {
        int idA = colliderA.id;
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
                        if (colliderA.type == ColliderType::DYNAMIC && colliderB.type == ColliderType::DYNAMIC) {
                            // If the two are dynamic, check weight
                            if (colliderA.weight > colliderB.weight) {
                                undoCollision(gameContext, colliderA, colliderB);
                            }
                            else {
                                undoCollision(gameContext, colliderB, colliderA);
                            }
                        }
                        else if (colliderA.type == ColliderType::DYNAMIC) {
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


    // First check the collision with the platform and check if we dont have to undoCollision
    bool undoColl = true;
    if (solidCol.collisionLayer == ColliderComponent::Platform) {
        if (abs(overlapX) >= abs(overlapY))
            undoColl = checkCollisionWithPlatform(gameContext, solidCol, mobileCol, sitSolid, sitMobile, overlapY);
        else
            return;
    }
    if (!undoColl)
        return;
   

    if ( overlapX == 0 || (overlapY != 0 && std::abs(overlapY) <= std::abs(overlapX)) ) {
        sitMobile.position.y += overlapY;

        // Set velocity to mobile
        if (mobileVel.velocity.y > 0.f) { // moving down
            if (sitSolid.position.y > sitMobile.position.y) {
                mobileVel.velocity.y = 0.f;
            }
        }
        else {                           // moving up
            if (sitSolid.position.y < sitMobile.position.y) {
                mobileVel.velocity.y = 0.f;
            }
        }

        if (solidCol.type == ColliderType::DYNAMIC && gameContext.entityMan.existsComponent<VelocityComponent>(solidCol.id)) {
            VelocityComponent& solidVel = gameContext.entityMan.getComponent<VelocityComponent>(solidCol.id);

            if (solidVel.velocity.y > 0.f) { // moving down
                if (sitSolid.position.y < sitMobile.position.y) {
                    solidVel.velocity.y = 0.f;
                }
            }
            else {                           // moving up
                if (sitSolid.position.y > sitMobile.position.y) {
                    solidVel.velocity.y = 0.f;
                }
            }
        }
    }
    else {
        sitMobile.position.x += overlapX;

        // Set velocity to mobile
        if (mobileVel.velocity.x > 0.f) { // moving right
            if (sitSolid.position.x > sitMobile.position.x) {
                mobileVel.velocity.x = 0.f;
            }
        }
        else {                           // moving left
            if (sitSolid.position.x < sitMobile.position.x) {
                mobileVel.velocity.x = 0.f;
            }
        }

        if (solidCol.type == ColliderType::DYNAMIC && gameContext.entityMan.existsComponent<VelocityComponent>(solidCol.id)) {
            VelocityComponent& solidVel = gameContext.entityMan.getComponent<VelocityComponent>(solidCol.id);
            
            // Set velocity to mobile
            if (solidVel.velocity.x > 0.f) { // moving right
                if (sitSolid.position.x < sitMobile.position.x) {
                    solidVel.velocity.x = 0.f;
                }
            }
            else {                           // moving left
                if (sitSolid.position.x > sitMobile.position.x) {
                    solidVel.velocity.x = 0.f;
                }
            }
        }
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


bool CollisionSystem::checkCollisionWithPlatform(GameEngine& gameContext, ColliderComponent& platformColl, ColliderComponent& entityColl, SituationComponent& platformSit, SituationComponent& entitySit, float overlapY) const {
    //std::cout << "platformSit y: " << platformSit.position.y << " entitySit y: " << entitySit.position.y << " entityColl yDown: " << entityColl.boundingRoot.bounding.yDown << " overlapY: " << abs(overlapY) << "\n";
    float bias = 4.9f; // This bias is for solving when you press down, cuanto más grande, el jugador tiene que pulsar durante más tiempo hacia abajo, pero si hay pocos fps o va muy rápido, puede que no coja al jugador cuando está encima de la plataforma
    if (abs(overlapY) > bias) {
        overlapY = bias;
    }
    // Todas las plataformas por encima de la parte inferior de la entidad no colisionan
    if (platformSit.position.y > (entitySit.position.y + entityColl.boundingRoot.bounding.yDown - abs(overlapY)-0.1f)) {
        // Cuando la entidad va hacia arriba, no colisiona
        if (gameContext.entityMan.existsComponent<VelocityComponent>(entityColl.id)) {
            if (gameContext.entityMan.getComponent<VelocityComponent>(entityColl.id).velocity.y >= 0.f) {
                // Por ultimo, si es el jugador y ha pulsado hacia abajo
                if (entityColl.id == WorldElementsData::playerId) {
                    if (gameContext.entityMan.getComponent<InputComponent>(entityColl.id).movingDown == false) {
                        return true;
                    }
                }
                else
                {
                    return true;
                }
            }
        }
    }

    return false;
}