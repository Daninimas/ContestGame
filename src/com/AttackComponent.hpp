#pragma once

#include "Component.hpp"

class AttackComponent : public Component {
public:
    explicit AttackComponent(const int id);
    ~AttackComponent() override;

    uint16_t damage{ 1 };
};

