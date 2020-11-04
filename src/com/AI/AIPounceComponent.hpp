#pragma once

#include "../Component.hpp"

class AIPounceComponent : public Component {
public:
    explicit AIPounceComponent(const int id);
    ~AIPounceComponent() override;


    int objectiveId;

    float rangeX{ 70.f };
    float rangeY{ 10.f };

    float velocityIncFactor{ 3.f };

    float cooldown{ 0.f };
    float maxCooldown{ 1.f };
};