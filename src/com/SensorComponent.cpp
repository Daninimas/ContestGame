#include "SensorComponent.hpp"

SensorComponent::SensorComponent(const int id)
    : Component(id), entitiesSensoring{} {
    
    entitiesSensoring.reserve(10);
}

SensorComponent::~SensorComponent() {}
