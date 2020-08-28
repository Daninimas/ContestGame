#include "VelocityComponent.hpp"

VelocityComponent::VelocityComponent(const int id)
    : Component(id) {}

VelocityComponent::~VelocityComponent() {}

void VelocityComponent::resetAllVelocities() {
    velocityX = 0.f;
    velocityY = 0.f;
}