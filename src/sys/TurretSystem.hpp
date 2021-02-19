#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class TurretSystem : public System {
public:
    TurretSystem();
    ~TurretSystem();

    void update(GameEngine& gameContext) const override;

private:
    void manageTurret(GameEngine& gameContext, TurretComponent& turret) const;
    void checkIfPlayerUsesTurret(GameEngine& gameContext, TurretComponent& turret) const;
    void enterInTurret(GameEngine& gameContext, TurretComponent& turret, int userID) const;

    void manageTurretUsage(GameEngine& gameContext, TurretComponent& turret) const;
    void exitTurret(GameEngine& gameContext, InputComponent& userInput, TurretComponent& turret) const;

};