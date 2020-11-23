#pragma once
#include "Component.hpp"

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
};

