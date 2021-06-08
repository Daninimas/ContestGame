#pragma once

#include "Component.hpp"
#include <vector>
#include <enum/MenuOptions.hpp>

class MenuComponent : public Component {
public:
    explicit MenuComponent(const int id);
    ~MenuComponent() override;

    std::vector<int> optionsId;
    std::size_t selectedOption {0};
    bool firstTime{ true };

    // For the text in menu
    std::vector<int> textsId;

};

