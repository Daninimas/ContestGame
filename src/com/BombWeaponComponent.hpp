#pragma once

#include "Component.hpp"
#include <com/WeaponComponent.hpp>


class BombWeaponComponent : public WeaponComponent {
public:
    explicit BombWeaponComponent(const int id);
    ~BombWeaponComponent() override;
    
    bool activated{ false };

    float explosionTime{ 1.f };
    float currentTime  { 0.f };

    float explosionExpansion;
    float explosionLifetime;
};