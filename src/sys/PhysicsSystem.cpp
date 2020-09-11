#include "PhysicsSystem.hpp"

#include <eng/GameEngine.hpp>


PhysicsSystem::PhysicsSystem() {}

PhysicsSystem::~PhysicsSystem() {}


void PhysicsSystem::update(GameEngine& gameContext) const {
    // Update the vel of the jumping entities
    updateJumps(gameContext);

    //Change the SituationComponent of the entities that moves
    updateSituations(gameContext);

    // Update situation in engine
    gameContext.getWindowFacadeRef().updateEntities(gameContext, gameContext.entityMan.getEntitiesToUpdate());

    // Reset entities to update
    gameContext, gameContext.entityMan.clearEntitiesToUpdate();
}

void PhysicsSystem::updateJumps(GameEngine& gameContext) const {
    auto& jumpsComponents = gameContext.entityMan.getComponents<JumpComponent>();

    for (JumpComponent& jump : jumpsComponents) {
        VelocityComponent& velocity = gameContext.entityMan.getComponent<VelocityComponent>(jump.id);
        // Manage cooldown to make it not jump on air
        // Count time than you are on the floor
        if (velocity.velocityY == 0.f) {
            jump.cooldow += gameContext.getDeltaTime();
        }
        else {
            jump.cooldow = 0.f;
        }


        if (jump.jumpIndex < jump.jumptable.size()) {

            velocity.velocityY -= jump.jumptable[jump.jumpIndex];
            ++jump.jumpIndex;
        }
    }
}


void PhysicsSystem::updateSituations(GameEngine& gameContext) const {
    //Use the deltaTime for interpolation
    const float deltaTime = gameContext.getDeltaTime();
    auto& velocities = gameContext.entityMan.getComponents<VelocityComponent>();


    for (VelocityComponent& velocity : velocities) {
        SituationComponent& situation = gameContext.entityMan.getComponent<SituationComponent>(velocity.id);

        // Gravity
        velocity.velocityY += velocity.gravity;
        velocity.velocityY = std::clamp(velocity.velocityY, velocity.minVy, velocity.maxVy);

        // Update positions
        situation.x = situation.x + velocity.velocityX * deltaTime;
        situation.y = situation.y + velocity.velocityY * deltaTime;

        gameContext.entityMan.addEntityToUpdate(velocity.id);
    }
}