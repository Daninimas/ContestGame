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
    std::vector<int> entitiesToUpdate = gameContext.entityMan.getEntitiesToUpdate();
    for (int id : entitiesToUpdate) {
        SituationComponent& situation = gameContext.entityMan.getComponent<SituationComponent>(id);
        VelocityComponent& velocity = gameContext.entityMan.getComponent<VelocityComponent>(id);

        situation.x = situation.x + velocity.velocityX * deltaTime;
        situation.y = situation.y + velocity.velocityY * deltaTime;
    }
}