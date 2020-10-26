#pragma once

#include "../Component.hpp"

class AIMeleeComponent : public Component {
public:
    explicit AIMeleeComponent(const int id);
    ~AIMeleeComponent() override;


    int objectiveId;

    bool createAttack{ false };
};