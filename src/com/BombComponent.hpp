#pragma once

#include "Component.hpp"


class BombComponent : public Component {
public:
    explicit BombComponent(const int id);
    ~BombComponent() override;
    
    bool activated{ false };

    float explosionTime{ 1.f };
    float currentTime  { 0.f };

    float explosionExpansion;
    float explosionLifetime;

    uint16_t damageExplosion{ 1 };
};