#include "HealthSystem.hpp"

#include <eng/GameEngine.hpp>
#include <enum/EntityType.hpp>

HealthSystem::HealthSystem() {}

HealthSystem::~HealthSystem() {}

void HealthSystem::update(GameEngine& gameContext) const {
    manageHeatlths(gameContext);
}

void HealthSystem::manageHeatlths(GameEngine& gameContext) const {
    auto& healths = gameContext.entityMan.getComponents<HeathComponent>();

    for (HeathComponent& health : healths) {
        // Substract damage
        if (health.damaged) {
            if (health.currentHealth < health.damageReceived) {
                health.currentHealth = 0;
            }
            else {
                health.currentHealth -= health.damageReceived;
            }


            // Reset damage data
            health.damaged = false;
            health.damageReceived = 0;
        }
    }
}