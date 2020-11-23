#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class MenuSystem : public System {
public:
    MenuSystem();
    ~MenuSystem();

    void update(GameEngine& gameContext) const override;

private:
    void selectOption(GameEngine& gameContext, MenuComponent& menuComp, std::size_t lastSelected) const;
    void acceptOption(GameEngine& gameContext, MenuComponent& menuComp) const;
};