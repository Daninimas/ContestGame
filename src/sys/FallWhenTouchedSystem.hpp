#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class FallWhenTouchedSystem : public System {
public:
    FallWhenTouchedSystem();
    ~FallWhenTouchedSystem();

    void update(GameEngine& gameContext) const override;

private:
    void setGravity(GameEngine& gameContext, FallWhenTouchedComponent& fallComp) const;

};