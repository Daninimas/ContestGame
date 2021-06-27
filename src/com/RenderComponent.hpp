#pragma once

#include <com/Component.hpp>
#include <tools/BoundingBox.hpp>
#include <tools/Color.hpp>
#include <string>

class RenderComponent : public Component
{
public:
    explicit RenderComponent(const int id);
    ~RenderComponent() override;

    std::string sprite = "";

    BoundingBox spriteRect{ 0.f, 10.f, 0.f, 10.f };

    Color color {255, 255, 255, 255};

    bool isHUDElement{ false };

    bool isRepeated{ false };
};

