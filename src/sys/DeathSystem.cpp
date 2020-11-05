#include "DeathSystem.hpp"

#include <eng/GameEngine.hpp>
#include <enum/EntityType.hpp>

#include <iostream>

DeathSystem::DeathSystem() {}

DeathSystem::~DeathSystem() {}

void DeathSystem::update(GameEngine& gameContext) const {
	std::vector <int> deadEntities = getDeadEntities(gameContext);

    deleteEntities(gameContext, deadEntities);
}

std::vector<int> DeathSystem::getDeadEntities(GameEngine& gameContext) const {
    auto& allHealths = gameContext.entityMan.getComponents<HealthComponent>();

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
        EntityType entityTypeToDelete = gameContext.entityMan.getEntity(entityId).getType();
        if (entityTypeToDelete != EntityType::PLAYER) {
            gameContext.eraseEntityByID(entityId); // Delete the entity
        }
        else {
            std::cout << "Has muerto\n";
        }
    }
}