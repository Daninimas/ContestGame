#pragma once

#include "../Component.hpp"

class AIDistanceAtkComponent : public Component {
public:
    explicit AIDistanceAtkComponent(const int id);
    ~AIDistanceAtkComponent() override;

    int objectiveId;

    bool createAttack{ false };

    float rangeX{ 200.f };
    float rangeY{ 20.f };
};