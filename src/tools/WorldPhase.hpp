#pragma once

#include <tools/BoundingBox.hpp>
#include <tools/Sound.hpp>

class WorldPhase {
public:
    WorldPhase() {};
    ~WorldPhase() = default;

    BoundingBox limits;

    enum {
        Left,
        Right,
        Up,
        Down
    };
    uint8_t direction{ Right }; // Direction of the phase where the end will be

    float endDistance{ 100.f }; // Min distance to the end limit of the phase to move on to the next phase

    //Sound phaseMusic{};
};