#pragma once

#include "Component.hpp"

class VelocityComponent : public Component {
public:
    explicit VelocityComponent(const int id);
    ~VelocityComponent() override;

    void resetAllVelocities();

    float	velocityX{ 0 };
    float	velocityY{ 0 };

    float	speed{ 70 };
};