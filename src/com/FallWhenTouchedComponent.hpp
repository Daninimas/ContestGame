#pragma once

#include "Component.hpp"
#include <limits>

class FallWhenTouchedComponent : public Component {
public:
    explicit FallWhenTouchedComponent(const int id);
    ~FallWhenTouchedComponent() override;

    float gravity{ 500.f };

    int objectiveId = std::numeric_limits<int>::max();

    bool activated{ false };

    float countdown{ 0.f };
    float maxCountdown{ 0.7f };
};
