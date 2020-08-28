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
};

