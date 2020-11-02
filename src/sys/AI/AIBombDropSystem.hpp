#pragma once

#include "../System.hpp"
#include <com/ComponentsIncluder>

class AIBombDropSystem : public System {
public:
    AIBombDropSystem();
    ~AIBombDropSystem();

    void update(GameEngine& gameContext) const override;

private:

};