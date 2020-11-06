#pragma once

#include <tools/Vector2.hpp>
#include "Component.hpp"

class SituationComponent : public Component {
public:
    explicit SituationComponent(const int id);
    ~SituationComponent() override;

    Vector2 position    { 0.f, 0.f };
    float	rotation	{0.f};

    Vector2	scale      { 1.f, 1.f };

    enum {
        Left,
        Right
    };
    uint8_t facing{ Right };
};
