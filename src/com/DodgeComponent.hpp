#pragma once
#include "Component.hpp"

class DodgeComponent : public Component {
public:
    explicit DodgeComponent(const int id);
    ~DodgeComponent() override;

    float cooldown{ 0.f };
    float maxCooldown{ 0.3f };

    float velocityIncrementFactor{ 2.f };
    uint8_t dodgeDirection{ Right };

    bool activateDodge{ false };

    float dodgeDuration{ 0.f };
    float dodgeMaxDuration{ 0.7f };


    // ---------------------- SOLO SI SE TIENE UN INPUT COMPONENT
    float dodgeTime{ 0.5f }; // El tiempo en el que tienes que pulsar dos veces la tecla de movimiento para activar el dodge
    float timeFromLastMove{ 0.f }; // Sirve para ver si se activa el dodge

    enum {
        Left,
        Right,
        Up,
        Down
    };
    uint8_t lastMovement{ Right };

    bool releasedKey{ false };




    void initDodgeComponent();
};

