#pragma once

#include "Component.hpp"

#include <vector>

class BoundingComponent : public Component {
public:
    explicit BoundingComponent(const int id);
    ~BoundingComponent() override;

    float width{ 1.f };
    float height{ 1.f };

    enum class LayerMasc
    {
        NoLayer = 0x00,
        Walls   = 0x01
    };

    LayerMasc layer{0xFF};

    std::vector<int> entitiesColliding;
};
