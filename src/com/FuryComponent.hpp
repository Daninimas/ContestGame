#pragma once

#include "Component.hpp"
#include <tools/Color.hpp>

class FuryComponent : public Component {
public:
    explicit FuryComponent(const int id);
    ~FuryComponent() override;

    float timersSpeedIncFactor{ 0.7f }; // incremento que hace que las animaciones o cooldowns vayan mas rapidos y taque mas rapido o cosas asi (hay que tener en cuanta que ya se ha hecho uno en su sistema correspondiente)

    float speedIncFactor{ 1.f };

    float totalLifeTime{ 5.f };
    float lifeTime{ 0.f };

    Color furyColor;
};
