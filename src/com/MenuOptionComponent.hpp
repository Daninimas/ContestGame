#pragma once

#include "Component.hpp"
#include <vector>
#include <enum/MenuOptions.hpp>

class MenuOptionComponent : public Component {
public:
    explicit MenuOptionComponent(const int id);
    ~MenuOptionComponent() override;

    MenuOptions option;

    bool active{ false };
};