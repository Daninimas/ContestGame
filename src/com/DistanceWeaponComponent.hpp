#pragma once
#include "Component.hpp"
#include <com/WeaponComponent.hpp>
#include <enum/AttackType.hpp>

class DistanceWeaponComponent : public WeaponComponent {
public:
    explicit DistanceWeaponComponent(const int id);
    ~DistanceWeaponComponent() override;

    float attackGeneralVelociy{ 10.f }; // the max velocity for setting the attack

    float attackVelX{ 0.f };
    float attackVelY{ 0.f };
    float attackGravity{ 0.f };

    enum
    {
        BULLET,
        BOMB, 
        LASER
    };

    uint8_t attackGeneratedType{ BULLET };


    // Only for bombs generation

    bool startActivated{ false };
    float explosionTime{0.f};
    float explosionExpansion{1.f};

    // Only for bombs generation
};
