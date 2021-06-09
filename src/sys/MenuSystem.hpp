#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>
#include <vector>
#include <string>

class MenuSystem : public System {
public:
    MenuSystem();
    ~MenuSystem();

    void update(GameEngine& gameContext) const override;

private:
    void updateNewBestScore(GameEngine& gameContext, MenuComponent& menuComp) const;
    void selectOption(GameEngine& gameContext, MenuComponent& menuComp, std::size_t lastSelected) const;
    void acceptOption(GameEngine& gameContext, MenuComponent& menuComp) const;
    void changeCharacter(GameEngine& gameContext, MenuComponent& menuComp, int step) const;
    void manageBestScoreAdding(GameEngine& gameContext, MenuComponent& menuComp) const;


    // Letras de las opciones
    const std::vector<std::string> characters{
        "A",
        "B",
        "C",
        "D",
        "E",
        "F",
        "G",
        "H",
        "I",
        "J",
        "K",
        "L",
        "M",
        "N",
        "O",
        "P",
        "Q",
        "R",
        "S",
        "T",
        "U",
        "V",
        "W",
        "X",
        "Y",
        "Z",
        "_"
    };
};