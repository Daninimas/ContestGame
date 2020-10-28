#pragma once

#include "../Component.hpp"

class AIMeleeAtkComponent : public Component {
public:
    explicit AIMeleeAtkComponent(const int id);
    ~AIMeleeAtkComponent() override;


    int objectiveId;

    bool createAttack{ false };
};