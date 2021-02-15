#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class OrbitalWeaponSystem : public System {
public:
    OrbitalWeaponSystem();
    ~OrbitalWeaponSystem();

    void update(GameEngine& gameContext) const override;

private:
    void addCooldownToOrbitalWeapons(GameEngine& gameContext) const;
    void checkEnemiesAttacking(GameEngine& gameContext) const;
    void manageOrbitalStrikes(GameEngine& gameContext) const;

    void generateOrbitalMarker(GameEngine& gameContext, OrbitalWeaponComponent& orbitalWeapon, SituationComponent& objectiveSituation) const;
    void generateOrbitalAttack(GameEngine& gameContext, OrbitalWeaponComponent& orbitalWeapon) const;

    void calculateBoundingToFloor(GameEngine& gameContext, SituationComponent& objectiveSituation, OrbitalWeaponComponent& orbitalWeapon) const;

};

