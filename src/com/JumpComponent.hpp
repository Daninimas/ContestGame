#pragma once

#include "Component.hpp"

#include <tools/Sound.hpp>

class JumpComponent : public Component {
public:
    explicit JumpComponent(const int id);
    ~JumpComponent() override;

    /*
    std::array<float, 8> jumptable;

    size_t jumpIndex{ jumptable.size() }; // if index is at the end of the jumptable, the entitiy is not jumping
    */

    float maxCooldown{ 0.7f };
    float cooldown{ 0.f };

    float impulse{ -50.f };

    Sound jumpSound;
};

