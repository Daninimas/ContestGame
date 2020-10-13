#pragma once
#include "Component.hpp"

class InputComponent : public Component {
public:
    explicit InputComponent(const int id);
    ~InputComponent() override;
    
    void resetActions();


    bool movingLeft  { false };
    bool movingRight { false };
    bool movingUp    { false };
    bool movingDown  { false };

    bool attacking   { false };
    //bool canAttack   { true };
    bool jumping     { false };

    bool select      { false };

    float coolDown    { 0.f }; // used on the menus
    float maxCoolDown { 0.5f };
};

