#pragma once

#include "Component.hpp"
#include <enum/GameObjectType.hpp>
#include <tools/Color.hpp>

class HealthComponent : public Component {
public:
    explicit HealthComponent(const int id);
    ~HealthComponent() override;

    void resetHealth();

    uint16_t    maxHealth{ 10 };
    uint16_t    currentHealth{ 10 };

    uint16_t    damageReceived{ 0 };
    bool        damaged{ false };

    // For the color when hitted
    float       hitColorCounter{0};
    float       hitColorMaxTime{0.3f};
    bool        isRedColored{ false };


    // When player loses a life
    uint8_t     extraLifes{ 0 }; // lifes for the player

    float       recoverTime{ 2.f }; // Time when the player is invincible after losing a life
    float       recoverTimeCounter{ 0.f };

    float       invincibleAnimTime{ 0.25f };
    float       invincibleAnimCounter{ 0.f };

    GameObjectType hittedByGO{ GameObjectType::NONE }; // The last Game Object that has hitted this entity


    // For the score system
    int score{ 100 };  // The score that gives to the player when this entity dies
};
