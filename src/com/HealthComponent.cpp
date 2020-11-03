#include "HealthComponent.hpp"

HealthComponent::HealthComponent(const int id)
    : Component(id) {}

HealthComponent::~HealthComponent() {}

void HealthComponent::resetHealth() {
    currentHealth = maxHealth;
}
