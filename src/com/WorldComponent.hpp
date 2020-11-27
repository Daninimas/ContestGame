#pragma once

#include "Component.hpp"
#include <tools/BoundingBox.hpp>
#include <tools/Sound.hpp>
#include <vector>

class WorldComponent : public Component {
public:
    explicit WorldComponent(const int id);
    ~WorldComponent() override;

    std::vector<BoundingBox> phaseLimits;
    std::size_t currentPhase{ 0 };

    Sound worldMusic{};
};