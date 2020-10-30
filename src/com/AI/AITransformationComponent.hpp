#pragma once

#include "../Component.hpp"
#include <tools/BoundingBox.hpp>
#include <string>
#include <array>

class AITransformationComponent : public Component {
public:
    explicit AITransformationComponent(const int id);
    ~AITransformationComponent() override;

    int objectiveId;

    float rangeX{ 70.f };
    float rangeY{ 70.f };


    // Data for the transformation
        // new collider
    BoundingBoxNode newBoundingRoot;

        // new render
    std::string newSprite = "";
    BoundingBox newSpriteRect{ 0.f, 10.f, 0.f, 10.f };
    std::array<uint8_t, 4> newColor{ 255, 255, 255, 255 };

        // new situation
    float	newScaleX{ 1.f };
    float	newScaleY{ 1.f };
};