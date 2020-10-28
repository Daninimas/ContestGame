#pragma once

#include "Component.hpp"

#include <tools/BoundingBox.hpp>
#include <enum/ColliderType.hpp>
#include <vector>

class CameraComponent : public Component {
public:
    explicit CameraComponent(const int id);
    ~CameraComponent() override;

    BoundingBox viewRect;

    float zoom{1.f};


    // Offset from the center where the player can be without moveng the camera
    float offsetX{100.f};
    float offsetY{100.f};
};