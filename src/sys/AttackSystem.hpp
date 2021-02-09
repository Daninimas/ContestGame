#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class AttackSystem : public System {
public:
    AttackSystem();
    ~AttackSystem();

    void update(GameEngine& gameContext) const override;

private:
    void manageAttacks(GameEngine& gameContext) const;
    void addCooldownTimeToWeapons(GameEngine& gameContext) const;
    void checkPlayerAttacking(GameEngine& gameContext) const;
    void checkEnemiesAttacking(GameEngine& gameContext) const;
    bool createMeleeAttack(GameEngine& gameContext, MeleeWeaponComponent& meleeAttacker) const;
    void animateExplosion(GameEngine& gameContext, AttackComponent& attack) const;
    void manageDamagePlatform(GameEngine& gameContext, AttackComponent& attack) const;
    bool createDistanceAttack(GameEngine& gameContext, DistanceWeaponComponent& distanceWeaponAttacker) const;
    void createBulletAttack(GameEngine& gameContext, DistanceWeaponComponent& distanceWeaponAttacker) const;
    void createBombEntity(GameEngine& gameContext, DistanceWeaponComponent& distanceWeaponAttacker) const;
    void createLaserAttack(GameEngine& gameContext, DistanceWeaponComponent& distanceWeaponAttacker) const;
    void createShotgunAttack(GameEngine& gameContext, DistanceWeaponComponent& distanceWeaponAttacker) const;
    void checkAttacksHits(GameEngine& gameContext) const;

    void resolveAttackHit(GameEngine& gameContext, ColliderComponent& attackCol, AttackComponent& attack, std::vector<int>& attacksToDelete) const;
    void damageEntity(GameEngine& gameContext, AttackComponent& attack, int entityHitId) const;

};

