#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class PickPowerUpSystem : public System {
public:
    PickPowerUpSystem();
    ~PickPowerUpSystem();

    void update(GameEngine& gameContext) const override;

private:
    int checkPowerUpCollides(GameEngine& gameContext, BoundingBoxNode& powerBounding) const;
    void setPowerUpToEntity(GameEngine& gameContext, PowerUpComponent& powerUp, int entityColliding, std::vector<int>& powerUpsToDelete) const;
    void setShieldToEntity(GameEngine& gameContext, PowerUpComponent& powerUp, int entityColliding) const;
    void setFuryToEntity(GameEngine& gameContext, PowerUpComponent& powerUp, int entityColliding) const;
    void displayPowerUpNameText(GameEngine& gameContext, PowerUpComponent& powerUp) const;

    const std::map<uint8_t, std::string> powerUpTextMap{
        // Miscellaneous
        { 0, "SHIELD" },
        { 1, "FURY" },
        { 2, "EXTRA LIFE" }
    };
};