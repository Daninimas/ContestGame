#pragma once
#include "Component.hpp"

class ExplosionAttackComponent : public Component {
public:
    explicit ExplosionAttackComponent(const int id);
    ~ExplosionAttackComponent() override;

    float expansionVelocity{1.f};
};
