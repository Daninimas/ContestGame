#pragma once

#include <com/Component.hpp>
#include <tools/BoundingBox.hpp>
#include <string>
#include <array>

class RenderComponent : public Component
{
public:
    explicit RenderComponent(const int id);
    ~RenderComponent() override;

    std::string sprite = "";

    BoundingBox spriteRect{ 0.f, 10.f, 0.f, 10.f };

    std::array <uint8_t, 4> color {255, 255, 255, 255};
};

