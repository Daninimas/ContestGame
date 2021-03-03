#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class DeathSystem : public System {
public:
    DeathSystem();
    ~DeathSystem();

    void update(GameEngine& gameContext) const override;

private:
    std::vector<int> getDeadEntities(GameEngine& gameContext) const;
    void deleteEntities(GameEngine& gameContext, std::vector<int>& deadEntities) const;

    void managePlayerLifes(GameEngine& gameContext, int playerId) const;

    void manageScore(GameEngine& gameContext, int deadEntityId) const;

    void disableTurret(GameEngine& gameContext, int turretID) const;
};