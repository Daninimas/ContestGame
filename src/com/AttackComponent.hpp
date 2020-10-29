#pragma once

#include "Component.hpp"
#include <enum/AttackType.hpp>
#include <vector>

class AttackComponent : public Component {
public:
    explicit AttackComponent(const int id);
    ~AttackComponent() override;

    uint16_t damage{ 1 };

    AttackType type{ AttackType::MELEE };

    float lifetime{ 0.f };
    float maxLifetime{ 0.5f }; // Time to delete the attack

    std::vector<int> entitiesDamaged;
};

