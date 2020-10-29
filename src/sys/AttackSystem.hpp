#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class AttackSystem : public System {
public:
    AttackSystem();
    ~AttackSystem();

    void update(GameEngine& gameContext) const override;

private:
    void deleteAttacksByTime(GameEngine& gameContext) const;
    void addCooldownTimeToWeapons(GameEngine& gameContext) const;
    void checkPlayerAttacking(GameEngine& gameContext) const;
    void checkEnemiesAttacking(GameEngine& gameContext) const;
    void createMeleeAttack(GameEngine& gameContext, MeleeWeaponComponent& meleeAttacker) const;
    void animateExplosions(GameEngine& gameContext) const;
    void createDistanceAttack(GameEngine& gameContext, DistanceWeaponComponent& distanceWeaponAttacker) const;
    void checkAttacksHits(GameEngine& gameContext) const;

    void resolveAttackHit(GameEngine& gameContext, ColliderComponent& attackCol, AttackComponent& attack, std::vector<int>& attacksToDelete) const;
    void damageEntity(GameEngine& gameContext, AttackComponent& attack, int entityHitId) const;

};

