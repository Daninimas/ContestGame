#pragma once

#include "Component.hpp"

class WeaponComponent : public Component {
public:
    explicit WeaponComponent(const int id);
    ~WeaponComponent() override;

    uint16_t damage{ 1 };

    float maxCooldown{ 0.5f };
    float cooldown{ 0.f };

    float attackWidth{ 1.f };
    float attacHeight{ 1.f };
};