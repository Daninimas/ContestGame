#include "SituationComponent.hpp"

SituationComponent::SituationComponent(const int id)
    : Component(id) {
    sons.reserve(3);
}

SituationComponent::~SituationComponent() {}
