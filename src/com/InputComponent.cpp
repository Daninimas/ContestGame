#include "InputComponent.hpp"

InputComponent::InputComponent(const int id) 
	: Component(id) {}

InputComponent::~InputComponent() {}

void InputComponent::resetActions() {
    movingLeft  = false ;
    movingRight = false ;
    movingUp    = false ;
    movingDown  = false ;
}
