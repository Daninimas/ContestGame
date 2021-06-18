#pragma once

#include "Component.hpp"
#include <vector>
#include <enum/MenuOptions.hpp>
#include <tools/Sound.hpp>

class MenuOptionComponent : public Component {
public:
    explicit MenuOptionComponent(const int id);
    ~MenuOptionComponent() override;

    MenuOptions option;

    bool active{ false };

    uint8_t characterSelected{ 0 }; // Solo para las opciones de letras de nueva mejor puntuacion

    // Sound
    Sound acceptSound;
};