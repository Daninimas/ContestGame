#pragma once

#include "Component.hpp"

class VelocityComponent : public Component {
public:
    explicit VelocityComponent(const int id);
    ~VelocityComponent() override;

    void resetAllVelocities();

    float	velocityX{ 0 };
    float	velocityY{ 0 };

    float	speedX{ 70 }; // this is the default velocity when walking (velocity constant)

    float minVy{ -500 }; // this is for making the entities acelerate with gravity
    float maxVy{ 500 };

    float   gravity{ 10 };
};