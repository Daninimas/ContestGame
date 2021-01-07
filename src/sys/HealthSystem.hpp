#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class HealthSystem : public System {
public:
    HealthSystem();
    ~HealthSystem();

    void update(GameEngine& gameContext) const override;

private:
    void checkPlayerInvincibility(GameEngine& gameContext) const;
    void manageHeatlths(GameEngine& gameContext) const;
};