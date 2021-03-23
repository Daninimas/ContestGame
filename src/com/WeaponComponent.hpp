#pragma once

#include "Component.hpp"
#include <tools/BoundingBox.hpp>
#include <tools/Sound.hpp>
#include <tools/Vector2.hpp>

class WeaponComponent : public Component {
public:
    explicit WeaponComponent(const int id);
    ~WeaponComponent() override;

    uint16_t damage{ 1 };

    float maxCooldown{ 0.7f };
    float cooldown{ 0.f };

    BoundingBox attackBounding;

    Sound attackSound{};

    float attackLifetime{ 0.5f };


    // Position where the attack is created from player
    Vector2 spawnAttackPos{ 0.f, 0.f }; // esto es con refencia a la esquina superior izquierda de la entidad (su posicion)

};