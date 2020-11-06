#pragma once

#include "../Component.hpp"
#include <tools/Vector2.hpp>

class AIDistanceAtkComponent : public Component {
public:
    explicit AIDistanceAtkComponent(const int id);
    ~AIDistanceAtkComponent() override;

    int objectiveId;

    bool createAttack{ false };

    Vector2 range{ 200.f, 20.f };
};