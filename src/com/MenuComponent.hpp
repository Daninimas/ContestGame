#pragma once

#include "Component.hpp"
#include <vector>
#include <enum/MenuOptions.hpp>

class MenuComponent : public Component {
public:
    explicit MenuComponent(const int id);
    ~MenuComponent() override;

    std::vector<MenuOptions> options;
    std::size_t selectedOption {0};
};

