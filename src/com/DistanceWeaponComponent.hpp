#pragma once
#include "Component.hpp"
#include <com/WeaponComponent.hpp>

class DistanceWeaponComponent : public WeaponComponent {
public:
    explicit DistanceWeaponComponent(const int id);
    ~DistanceWeaponComponent() override;

    float attackVelocity{ 10.f };
    float attackGravity{ 0.f };
};
