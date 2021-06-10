#pragma once

#include "Component.hpp"

class CheckpointComponent : public Component {
public:
    explicit CheckpointComponent(const int id);
    ~CheckpointComponent() override;

    bool active{ false };
};