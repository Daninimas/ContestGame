#pragma once
#include "Component.hpp"
#include <com/WeaponComponent.hpp>

class MeleeWeaponComponent : public WeaponComponent {
public:
    explicit MeleeWeaponComponent(const int id);
    ~MeleeWeaponComponent() override;
};

