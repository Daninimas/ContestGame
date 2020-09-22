#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class HealthSystem : public System {
public:
    HealthSystem();
    ~HealthSystem();

    void update(GameEngine& gameContext) const override;

private:
    void manageHeatlths(GameEngine& gameContext) const;
};