#pragma once

#include "Component.hpp"
#include <Enum/AnimationEnum.hpp>
#include <tools/BoundingBox.hpp>
#include <vector>
#include <string>

class AnimationComponent : public Component {
public:
    explicit AnimationComponent(const int id);
    ~AnimationComponent() override;


    AnimationEnum animation;

    std::vector<BoundingBox> frames;
    size_t actualFrame{ 0 };

    float framerate{ 0.2f };  // Time to change the frame
    float currentTime{ 0.f }; // Actual time for changing the frame

    bool repeat{ true };
};

