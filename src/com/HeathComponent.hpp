#pragma once

#include "Component.hpp"

class HeathComponent : public Component {
public:
    explicit HeathComponent(const int id);
    ~HeathComponent() override;

    void resetHealth();

    uint16_t    maxHealth{ 10 };
    uint16_t    currentHealth{ 10 };

    uint16_t    damageReceived{ 0 };
    bool        damaged{ false };
};
