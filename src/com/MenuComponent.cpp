#include "MenuComponent.hpp"

MenuComponent::MenuComponent(const int id)
	: Component(id), optionsId{} {
	optionsId.reserve(20);
}

MenuComponent::~MenuComponent() {}