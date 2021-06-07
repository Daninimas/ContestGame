#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class TimeSystem : public System {
public:
    TimeSystem();
    ~TimeSystem();

    void update(GameEngine& gameContext) const override;

private:

};