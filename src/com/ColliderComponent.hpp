#pragma once

#include "Component.hpp"

#include <tools/BoundingBox.hpp>
#include <vector>

class ColliderComponent : public Component {
public:
    explicit ColliderComponent(const int id);
    ~ColliderComponent() override;

    enum class LayerMasc
    {
        NoLayer = 0x00,
        Walls   = 0x01
    };

    LayerMasc layer{0xFF};

    BoundingBoxNode boundingRoot;
};
