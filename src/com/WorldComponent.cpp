#include "WorldComponent.hpp"

WorldComponent::WorldComponent(const int id)
    : Component(id), phaseLimits() {
    phaseLimits.reserve(16);
}

WorldComponent::~WorldComponent() {}