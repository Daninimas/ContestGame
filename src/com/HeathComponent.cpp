#include "HeathComponent.hpp"

HeathComponent::HeathComponent(const int id)
    : Component(id) {}

HeathComponent::~HeathComponent() {}

void HeathComponent::resetHealth() {
    currentHealth = maxHealth;
}
