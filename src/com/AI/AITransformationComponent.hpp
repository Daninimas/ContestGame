#pragma once

#include "../Component.hpp"
#include <tools/BoundingBox.hpp>
#include <tools/Vector2.hpp>
#include <tools/Color.hpp>
#include <string>
#include <array>

class AITransformationComponent : public Component {
public:
    explicit AITransformationComponent(const int id);
    ~AITransformationComponent() override;

    int objectiveId;

    Vector2 range{ 70.f, 70.f };

    // Data for the transformation
        // new collider
    BoundingBoxNode newBoundingRoot;

        // new render
    std::string newSprite = "";
    BoundingBox newSpriteRect{ 0.f, 10.f, 0.f, 10.f };
    Color newColor{ 255, 255, 255, 255 };

        // new situation
    Vector2 newScale{ 1.f, 1.f };
};