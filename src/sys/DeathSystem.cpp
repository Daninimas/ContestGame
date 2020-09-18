#include "DeathSystem.hpp"

#include <eng/GameEngine.hpp>
#include <enum/EntityType.hpp>

DeathSystem::DeathSystem() {}

DeathSystem::~DeathSystem() {}

void DeathSystem::update(GameEngine& gameContext) const {
    cout << "DENTRO DE Death 0 Map size: " << gameContext.entityMan.getComponents<RenderComponent>().sizeMap() << "\n";
    std::vector <int> deadEntities = getDeadEntities(gameContext);

    deleteEntities(gameContext, deadEntities);
    cout << "DENTRO DE Death 1 Map size: " << gameContext.entityMan.getComponents<RenderComponent>().sizeMap() << "\n";
}

std::vector<int> DeathSystem::getDeadEntities(GameEngine& gameContext) const {
    auto& allHeaths = gameContext.entityMan.getComponents<HeathComponent>();

    std::vector<int> deadEntities;
    deadEntities.reserve(allHeaths.size());

    for (auto& heath : allHeaths) {
        auto entityId{ heath.id };
        auto& heathComp{ heath };

        if (heathComp.currentHealth == 0) {
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
    }
}