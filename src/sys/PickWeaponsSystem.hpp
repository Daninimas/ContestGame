#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class PickWeaponsSystem : public System {
public:
    PickWeaponsSystem();
    ~PickWeaponsSystem();

    void update(GameEngine& gameContext) const override;

private:
    void checkPlayerPickingWeapons(GameEngine& gameContext) const;

    //bool checkWeaponCollidesWithPlayer(BoundingBoxNode& weaponBounding) const;

    void setDistanceWeaponToPlayer(GameEngine& gameContext, int distanceWeapId) const;
    void setMeleeWeaponToPlayer(GameEngine& gameContext, int meleeWeapId) const;
};