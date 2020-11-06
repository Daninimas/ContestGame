#pragma once

#include "Component.hpp"
#include <tools/Vector2.hpp>

class ShieldComponent : public Component {
public:
    explicit ShieldComponent(const int id);
    ~ShieldComponent() override;

    int objectiveId;

    Vector2 center; // center position of the shield relative to situation, this is storaged for optimization

    // Time to the shield?
};