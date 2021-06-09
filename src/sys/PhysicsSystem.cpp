#include "PhysicsSystem.hpp"

#include <eng/GameEngine.hpp>

#include <iostream>


PhysicsSystem::PhysicsSystem() {}

PhysicsSystem::~PhysicsSystem() {}


void PhysicsSystem::update(GameEngine& gameContext) const {
    // Update the vel of the jumping entities
    updateJumps(gameContext);

    //Change the SituationComponent of the entities that moves
    updateSituations(gameContext);
}

void PhysicsSystem::updateJumps(GameEngine& gameContext) const {
    auto& jumpsComponents = gameContext.entityMan->getComponents<JumpComponent>();

    for (JumpComponent& jump : jumpsComponents) {
        VelocityComponent& velocity = gameContext.entityMan->getComponent<VelocityComponent>(jump.id);
        // Manage cooldown to make it not jump on air
        // Count time than you are on the floor
        if (velocity.velocity.y == 0.f) {
            jump.cooldown += gameContext.getDeltaTime();
        }
        else {
            jump.cooldown = 0.f;
        }

        /*if (jump.jumpIndex < jump.jumptable.size()) {
            velocity.velocityY -= jump.jumptable[jump.jumpIndex];
            ++jump.jumpIndex;
        }*/
    }
}


void PhysicsSystem::updateSituations(GameEngine& gameContext) const {
    //Use the deltaTime for interpolation
    const float deltaTime = gameContext.getDeltaTime();
    auto& velocities = gameContext.entityMan->getComponents<VelocityComponent>();


    for (VelocityComponent& velocity : velocities) {
        SituationComponent& situation = gameContext.entityMan->getComponent<SituationComponent>(velocity.id);

        // Set the facing
        if (velocity.velocity.x > 0) {
            situation.facing = SituationComponent::Right;
        }
        else if(velocity.velocity.x < 0) {
            situation.facing = SituationComponent::Left;
        }
        // if == 0, is the same as before


        // Gravity
        velocity.velocity.y += velocity.gravity * deltaTime;
        velocity.velocity.y = std::clamp(velocity.velocity.y, velocity.minVy, velocity.maxVy);

        // Update positions
        situation.position.x = situation.position.x + velocity.velocity.x * deltaTime;
        situation.position.y = situation.position.y + velocity.velocity.y * deltaTime;

        gameContext.entityMan->addEntityToUpdate(velocity.id);
    }
}