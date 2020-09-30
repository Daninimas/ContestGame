#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class CameraSystem : public System {
public:
    CameraSystem();
    ~CameraSystem();

    void update(GameEngine& gameContext) const override;

private:
    
};