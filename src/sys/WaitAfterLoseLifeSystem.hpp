#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class WaitAfterLoseLifeSystem : public System {
public:
    WaitAfterLoseLifeSystem();
    ~WaitAfterLoseLifeSystem();

    void update(GameEngine& gameContext) const override;

private:

};