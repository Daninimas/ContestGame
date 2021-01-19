#pragma once

#include "../Component.hpp"
#include <tools/Vector2.hpp>

class AIPounceComponent : public Component {
public:
    explicit AIPounceComponent(const int id);
    ~AIPounceComponent() override;


    int objectiveId;

    Vector2 range{ 70.f, 10.f };

    float velocityIncFactor{ 3.f };

    float cooldown{ 0.f };
    float maxCooldown{ 1.f };

    bool isStickyPouncer{ false }; // If is sticky, the enemy will pounce to the objective and the be sticked to it until someone dies

    // For the sticky pouncer 
    bool sticked{ false };
};