#pragma once

#include "Component.hpp"

class HealthComponent : public Component {
public:
    explicit HealthComponent(const int id);
    ~HealthComponent() override;

    void resetHealth();

    uint16_t    maxHealth{ 10 };
    uint16_t    currentHealth{ 10 };

    uint16_t    damageReceived{ 0 };
    bool        damaged{ false };

    // When player loses a life
    uint8_t     extraLifes{ 0 }; // lifes for the player

    float       recoverTime{ 2.f }; // Time when the player is invincible after losing a life
    float       recoverTimeCounter{ 0.f };

    float       invincibleAnimTime{ 0.25f };
    float       invincibleAnimCounter{ 0.f };
};
