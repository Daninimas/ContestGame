#pragma once

#include "Component.hpp"
#include <tools/BoundingBox.hpp>
#include <tools/Sound.hpp>

class WeaponComponent : public Component {
public:
    explicit WeaponComponent(const int id);
    ~WeaponComponent() override;

    uint16_t damage{ 1 };

    float maxCooldown{ 0.7f };
    float cooldown{ 0.f };

    BoundingBox attackBounding;

    Sound attackSound{};

    /*
    // Position where the attack is created from player
    float spawnX{ 0.f };
    float spawnY{ 0.f };
    */
};