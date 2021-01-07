#include "HealthSystem.hpp"

#include <eng/GameEngine.hpp>
#include <enum/EntityType.hpp>

HealthSystem::HealthSystem() {}

HealthSystem::~HealthSystem() {}

void HealthSystem::update(GameEngine& gameContext) const {
    checkPlayerInvincibility(gameContext);

    manageHeatlths(gameContext);
}

void HealthSystem::checkPlayerInvincibility(GameEngine& gameContext) const {
    HealthComponent& playerHealth = gameContext.entityMan.getComponent<HealthComponent>(WorldElementsData::playerId);

    if (playerHealth.recoverTimeCounter < playerHealth.recoverTime) {
        playerHealth.recoverTimeCounter += gameContext.getDeltaTime();

        playerHealth.damaged = false;
        playerHealth.damageReceived = 0;


        //Do invincibility animation
        RenderComponent& playerRender = gameContext.entityMan.getComponent<RenderComponent>(WorldElementsData::playerId);

        playerHealth.invincibleAnimCounter += gameContext.getDeltaTime();
        Color playerAnimColor = playerRender.color;

        if (playerHealth.invincibleAnimCounter < playerHealth.invincibleAnimTime) {
            playerAnimColor.a = 0;
        }
        else if(playerHealth.invincibleAnimCounter > playerHealth.invincibleAnimTime*2.f){
            playerHealth.invincibleAnimCounter = 0.f;
        }

        gameContext.getWindowFacadeRef().setColorToEntity(WorldElementsData::playerId, playerAnimColor);
    }
}


void HealthSystem::manageHeatlths(GameEngine& gameContext) const {
    auto& healths = gameContext.entityMan.getComponents<HealthComponent>();

    for (HealthComponent& health : healths) {
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