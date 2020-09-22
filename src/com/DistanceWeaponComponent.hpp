#pragma once
#include "Component.hpp"
#include <com/WeaponComponent.hpp>

class DistanceWeaponComponent : public WeaponComponent {
public:
    explicit DistanceWeaponComponent(const int id);
    ~DistanceWeaponComponent() override;

    float attackGeneralVelociy{ 10.f }; // the max velocity for setting the attack

    float attackVelX{ 0.f };
    float attackVelY{ 0.f };
    float attackGravity{ 0.f };
};
