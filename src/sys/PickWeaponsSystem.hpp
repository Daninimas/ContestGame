#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>
#include <unordered_map>

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

    void displayWeaponNameText(GameEngine& gameContext, int weaponId) const;

    const std::map<GameObjectType, std::string> weaponTextMap{
        // Miscellaneous
        { GameObjectType::M4, "MACHINEGUN" },
        { GameObjectType::KNIFE, "KNIFE" },
        { GameObjectType::GRENADE_LAUNCHER, "GRENADE_LAUNCHER" },
        { GameObjectType::LASER_GUN, "LASER" },
        { GameObjectType::PISTOL, "PISTOL" },
        { GameObjectType::SHOTGUN, "SHOTGUN" }
    };
};