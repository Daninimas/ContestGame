#include "AnimationComponent.hpp"

AnimationComponent::AnimationComponent(const int id)
	: Component(id), frames() {
	frames.reserve(10);
}

AnimationComponent::~AnimationComponent() {}