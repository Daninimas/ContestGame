#include "VelocityComponent.hpp"

VelocityComponent::VelocityComponent(const int id)
    : Component(id) {}

VelocityComponent::~VelocityComponent() {}

void VelocityComponent::resetAllVelocities() {
    velocity = { 0.f, 0.f };
}