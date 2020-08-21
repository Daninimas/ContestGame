#pragma once
#include "Component.hpp"
#include <string>

class DrawableComponent : public Component {
public:
    explicit DrawableComponent(const int id);
    ~DrawableComponent() override;

    std::string sprite{ "" };
};

