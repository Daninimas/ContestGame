#include "MenuComponent.hpp"

MenuComponent::MenuComponent(const int id)
	: Component(id), options{} {
	options.reserve(20);
}

MenuComponent::~MenuComponent() {}