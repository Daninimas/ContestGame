#pragma once

#include "../Component.hpp"

class AIDropBombComponent : public Component {
public:
    explicit AIDropBombComponent(const int id);
    ~AIDropBombComponent() override;

    // Creates a bomb every X time
    float cooldown{ 0.f };
    float maxCooldown{ 2.f };

    bool createBomb{ false };
};