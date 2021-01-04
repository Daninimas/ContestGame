#pragma once

#include "Component.hpp"
#include <enum/Controls.hpp>
#include <map>

class InputComponent : public Component {
public:
    explicit InputComponent(const int id);
    ~InputComponent() override;
    
    void resetActions();


    bool movingLeft  { false };
    bool movingRight { false };
    bool movingUp    { false };
    bool movingDown  { false };

    bool attacking   { false };
    //bool canAttack   { true };
    bool jumping     { false };

    bool select      { false };

    float cooldown{ 0.f };
    float maxCooldown{ 0.4f };

    float dodgeTime{ 0.5f }; // El tiempo en el que tienes que pulsar dos veces la tecla de vomimiento para activar el dodge
    float timeFromLastMove{ 0.f }; // Sirve para ver si se activa el dodge

    uint8_t actualMovement = 0xFF; // start with no movement

    // ------------------- For the controls
    std::map<Controls, uint8_t> keyboardControlsMap{
        { Controls::MOVE_LEFT, 0 },   // A
        { Controls::MOVE_RIGHT, 3 },  // D
        { Controls::MOVE_UP, 22 },    // W
        { Controls::MOVE_DOWN, 18 },  // S
        { Controls::ACTION, 13 },     // N
        { Controls::JUMP, 12 },       // M
        // For the joystick
        { Controls::JOYSTICK_ACTION, 1 },
        { Controls::JOYSTICK_JUMP, 0 }
    };


    Controls controlToChange; // When we change a control

    // Para que no se chafen los dos inputs de teclado y joystick
    bool movedWithKeyboard{ false };
};

