#pragma once

#include "Component.hpp"
#include <tools/Vector2.hpp>

class TurretComponent : public Component {
public:
    explicit TurretComponent(const int id);
    ~TurretComponent() override;

    float rotationVelocity{20.f}; // Rotation velocity of the cannon

    int userID; //The entity using the turret
    
    bool inUse{ false }; //If there is an entity using this turret

    float offsetX{ 0.f }; // The offset from the x turret position where the user is moved
};