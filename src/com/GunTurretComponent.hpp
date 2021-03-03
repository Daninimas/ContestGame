#pragma once

#include "Component.hpp"

class GunTurretComponent : public Component {
public:
    explicit GunTurretComponent(const int id);
    ~GunTurretComponent() override;

    float gunRotationSpeed{ 10.f };

    bool createAttack{ false };

    float currentRotation{ 0.f };

    float maxRotation{ 90.f };
    float minRotation{ -10.f };
};