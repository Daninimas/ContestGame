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


    Animation animation = Animation::NONE;

    uint16_t totalFrames;
    uint16_t actualFrame{ 0 };

    BoundingBox startSpriteRect; // The spriteRect where the animation starts
    int nextFrameAdvance{ 512 }; // number of pixels that has to advance the spriteRect for the next frame

    float framerate{ 0.2f };  // Time to change the frame
    float currentTime{ 0.f }; // Actual time for changing the frame

    bool repeat{ true };
};

