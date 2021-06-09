#include "DeathSystem.hpp"

#include <eng/GameEngine.hpp>
#include <enum/EntityType.hpp>
#include <tools/Utils.hpp>

#include <iostream>

DeathSystem::DeathSystem() {}

DeathSystem::~DeathSystem() {}

void DeathSystem::update(GameEngine& gameContext) const {
	std::vector <int> deadEntities = getDeadEntities(gameContext);

    deleteEntities(gameContext, deadEntities);
}

std::vector<int> DeathSystem::getDeadEntities(GameEngine& gameContext) const {
    auto& allHealths = gameContext.entityMan->getComponents<HealthComponent>();

    std::vector<int> deadEntities;
    deadEntities.reserve(allHealths.size());

    for (auto& health : allHealths) {
        auto entityId{ health.id };
        auto& healthComp{ health };

        if (healthComp.currentHealth == 0) {
            deadEntities.emplace_back(entityId);
        }
    }
    return deadEntities;
}

void DeathSystem::deleteEntities(GameEngine& gameContext, std::vector<int>& deadEntities) const {
    for (int entityId : deadEntities) {
        EntityType entityTypeToDelete = gameContext.entityMan->getEntity(entityId).getType();

        switch (entityTypeToDelete)
        {
        case EntityType::PLAYER:
            managePlayerLifes(gameContext, entityId);
            break;

        case EntityType::TURRET:
            disableTurret(gameContext, entityId);
            break;

        default:
            // Give score to player
            manageScore(gameContext, entityId);
            gameContext.eraseEntityByID(entityId); // Delete the entity
            break;
        }
    }
}

void DeathSystem::managePlayerLifes(GameEngine& gameContext, int playerId) const {
    HealthComponent& healthComp = gameContext.entityMan->getComponent<HealthComponent>(playerId);

    if (healthComp.extraLifes > 0) {
        // Reset player stats when life lost
        --healthComp.extraLifes;
        healthComp.currentHealth = healthComp.maxHealth;
        healthComp.recoverTimeCounter = 0.f; 

        gameContext.entityMan->getComponent<InputComponent>(playerId).usingTurret = false;

        Utils::resetPlayerPosition(gameContext);
        gameContext.pushGameState(GameState::WAIT_AFTER_LOSE_LIFE);
    }
    else {
        gameContext.pushGameState(GameState::GAMEOVER);
    }
}


void DeathSystem::manageScore(GameEngine& gameContext, int deadEntityId) const {
    HealthComponent& deadEntityHealth = gameContext.entityMan->getComponent<HealthComponent>(deadEntityId);

    if (deadEntityHealth.hittedByGO == GameObjectType::PLAYER_MELEE_ATTACK || deadEntityHealth.hittedByGO == GameObjectType::PLAYER_DISTANCE_ATTACK || deadEntityHealth.hittedByGO == GameObjectType::PLAYER_EXPLOSION || deadEntityHealth.hittedByGO == GameObjectType::PLAYER_LASER || deadEntityHealth.hittedByGO == GameObjectType::PLAYER_SHIELD) {
        WorldElementsData::playerScore += deadEntityHealth.score;
    }
    else if (deadEntityHealth.hittedByGO == GameObjectType::NONE) {
        cout << "-------------------- ERROR: Entidad asesinada por GameObjectType = NONE --------------------\n";
        cout << "ID entidad: " << deadEntityId << "\n";
        cout << "Game Object Entidad: " << (int)gameContext.entityMan->getEntity(deadEntityId).getGameObjectType() << "\n";

    }
}


void DeathSystem::disableTurret(GameEngine& gameContext, int turretID) const {
    // This entity can be a turret platform or a turret canon
    if (gameContext.entityMan->existsComponent<TurretComponent>(turretID)) {
        TurretComponent& turretComp = gameContext.entityMan->getComponent<TurretComponent>(turretID);

        turretComp.disabled = true;
        if (turretComp.inUse) {
            gameContext.entityMan->getComponent<InputComponent>(turretComp.userID).usingTurret = false;
        }

        gameContext.entityMan->eraseComponent<ColliderComponent>(turretID);
    }
}
