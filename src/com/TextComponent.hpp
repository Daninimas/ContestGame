#pragma once

#include <com/Component.hpp>
#include <tools/Color.hpp>
#include <string>

class TextComponent : public Component
{
public:
    explicit TextComponent(const int id);
    ~TextComponent() override;

    std::string text = "";

    Color color{ 255, 255, 255, 255 };

    bool isHUDElement{ false };

    uint16_t size{ 12 };
};

