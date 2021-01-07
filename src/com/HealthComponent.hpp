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

    uint8_t     extraLifes{ 0 }; // lifes for the player
};
