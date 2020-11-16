#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class HUDSystem : public System {
public:
    HUDSystem();
    ~HUDSystem();

    void update(GameEngine& gameContext) const override;

private:
    void updateTexts(GameEngine& gameContext) const;

};