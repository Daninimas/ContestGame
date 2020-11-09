#pragma once

#include "Component.hpp"
#include <tools/Vector2.hpp>

class ShieldComponent : public Component {
public:
    explicit ShieldComponent(const int id);
    ~ShieldComponent() override;

    int objectiveId;

    //float colliderIncFactor{ 1.f }; // Always more than 1. It make the shield collider be bigger than the objective collider

    Vector2 center; // center position of the shield relative to situation, this is storaged for optimization

    // Time to the shield?
};