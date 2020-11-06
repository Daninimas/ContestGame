#pragma once
#include "Component.hpp"
#include <com/WeaponComponent.hpp>
#include <enum/AttackType.hpp>
#include <tools/Vector2.hpp>

class DistanceWeaponComponent : public WeaponComponent {
public:
    explicit DistanceWeaponComponent(const int id);
    ~DistanceWeaponComponent() override;

    float attackGeneralVelociy{ 10.f }; // the max velocity for setting the attack

    Vector2 attackVel;
    float attackGravity{ 0.f };

    enum
    {
        BULLET,
        BOMB, 
        LASER
    };

    uint8_t attackGeneratedType{ BULLET };

    uint16_t ammo{ 100 };
    bool infiniteAmmo{ false };


    // Only for bombs generation

    bool startActivated{ false };
    float explosionTime{0.f};
    float explosionExpansion{1.f};

    // Only for bombs generation
};
