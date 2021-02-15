#pragma once

#include "../Component.hpp"
#include <tools/Vector2.hpp>

class AIOrbitalAtkComponent : public Component {
public:
    explicit AIOrbitalAtkComponent(const int id);
    ~AIOrbitalAtkComponent() override;

    int objectiveId;
};