#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class AttackSystem : public System {
public:
    AttackSystem();
    ~AttackSystem();

    void update(GameEngine& gameContext) const override;

private:
    void addCooldownTimeToWeapons(GameEngine& gameContext) const;
    void checkPlayerAttacking(GameEngine& gameContext) const;
    void createMeleeAttack(GameEngine& gameContext, int attackerID) const;
};

