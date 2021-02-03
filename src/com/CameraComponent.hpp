#pragma once

#include "Component.hpp"

#include <tools/BoundingBox.hpp>
#include <enum/ColliderType.hpp>
#include <tools/Vector2.hpp>
#include <vector>

class CameraComponent : public Component {
public:
    explicit CameraComponent(const int id);
    ~CameraComponent() override;

    Vector2 viewRect;

    float zoom{1.f};

    float cameraAdvancement{ 100.f }; // This makes the camera to be more to the right from the player

    // Offset from the center where the player can be without moving the camera
    Vector2 offset{ 100.f, 100.f };
};