#pragma once
#include "Component.hpp"
#include <com/WeaponComponent.hpp>

class OrbitalWeaponComponent : public WeaponComponent {
public:
    explicit OrbitalWeaponComponent(const int id);
    ~OrbitalWeaponComponent() override;

    float generateAttackTime{ 1.f };
    float generateAttackTimeCounter{ 0.f };

    bool activated{ false };

    Sound markerSound{};
};

