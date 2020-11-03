#include "DodgeComponent.hpp"

DodgeComponent::DodgeComponent(const int id)
    : Component(id) {}

DodgeComponent::~DodgeComponent() {}

void DodgeComponent::initDodgeComponent() {
    timeFromLastMove = dodgeTime;
    dodgeDuration = dodgeMaxDuration;
}