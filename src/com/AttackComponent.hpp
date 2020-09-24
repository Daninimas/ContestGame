#pragma once

#include "Component.hpp"
#include<enum/AttackType.hpp>

class AttackComponent : public Component {
public:
    explicit AttackComponent(const int id);
    ~AttackComponent() override;

    uint16_t damage{ 1 };

    AttackType type{ AttackType::MELEE };

    uint8_t attackLifetime{ 2 }; // Necesita dos updates para que se borre, porque si no, no llega a quitar vida, ya que debe ( -crearse, colisionar- -quitar vida-, -destruirse-)
};

