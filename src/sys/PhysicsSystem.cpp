#include "PhysicsSystem.hpp"

#include <eng/GameEngine.hpp>

PhysicsSystem::PhysicsSystem() {}

PhysicsSystem::~PhysicsSystem() {}


void PhysicsSystem::update(GameEngine& gameContext) const {

    //Change the SituationComponent of the entities that moves
    updateSituations(gameContext);

    // Update situation in engine
    gameContext.getWindowFacadeRef().updateEntities(gameContext, gameContext.entityMan.getEntitiesToUpdate());

    // Reset entities to update
    gameContext, gameContext.entityMan.clearEntitiesToUpdate();
}


void PhysicsSystem::updateSituations(GameEngine& gameContext) const {
    //Use the deltaTime for interpolation
    const float deltaTime = gameContext.getDeltaTime();
    auto& velocities = gameContext.entityMan.getComponents<VelocityComponent>();


    for (VelocityComponent velocity : velocities) {
        SituationComponent& situation = gameContext.entityMan.getComponent<SituationComponent>(velocity.id);

        situation.x = situation.x + velocity.velocityX * deltaTime;
        situation.y = situation.y + (velocity.velocityY + velocity.gravity) * deltaTime;

        gameContext.entityMan.addEntityToUpdate(velocity.id);
    }
}