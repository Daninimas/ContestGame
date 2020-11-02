#pragma once

#include "../Component.hpp"

class AIChaseComponent : public Component {
public:
    explicit AIChaseComponent(const int id);
    ~AIChaseComponent() override;


    int objectiveId;

    float minDistanceX{ 1.f };
};