#include "InputComponent.hpp"

InputComponent::InputComponent(const int id) 
    : Component(id), timeFromLastMove{dodgeTime} {}

InputComponent::~InputComponent() {}

void InputComponent::resetActions() {
    movingLeft  = false ;
    movingRight = false ;
    movingUp    = false ;
    movingDown  = false ;

    attacking   = false;
    jumping     = false;
    select      = false;
}
