#pragma once

#include "Component.hpp"

#include <array>

class JumpComponent : public Component {
public:
    explicit JumpComponent(const int id);
    ~JumpComponent() override;

    std::array<float, 8> jumptable;

    size_t jumpIndex{ jumptable.size() }; // if index is ar the end of the jumptable, the entitiy is not jumping
};

