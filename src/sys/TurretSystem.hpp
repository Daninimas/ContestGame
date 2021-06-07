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

    void manageShoot(GameEngine& gameContext, TurretComponent& turret) const;
    void manageGunRotation(GameEngine& gameContext, TurretComponent& turret, InputComponent& userInput) const;

    void showText(GameEngine& gameContext, TextComponent& textComp, TurretComponent& turret) const;
    void hideText(GameEngine& gameContext, TurretComponent& turretComp) const;
};