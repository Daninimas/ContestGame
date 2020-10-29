#include "AttackComponent.hpp"

AttackComponent::AttackComponent(const int id)
	: Component(id), entitiesDamaged() {
	entitiesDamaged.reserve(10);
}

AttackComponent::~AttackComponent() {}