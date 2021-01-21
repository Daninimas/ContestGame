#pragma once

#include "../Component.hpp"

class AIFlyingChaseComponent : public Component {
public:
    explicit AIFlyingChaseComponent(const int id);
    ~AIFlyingChaseComponent() override;

    int objectiveId;

    float minDistanceX{ 1.f };
    float minHeigtht{ 1.f };
    float maxHeigtht{ 1.f };

    // For the flying up and down animation
    /*float animationMovementTime{ 2.f };
    float animationMovementTimeCounter{ 0.f };
    float animationVelocity{ 10.f };*/
};

