#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class BombSystem : public System {
public:
    BombSystem();
    ~BombSystem();

    void update(GameEngine& gameContext) const override;

private:
    void updateBombTimers(GameEngine& gameContext, BombWeaponComponent& bombComp, std::vector<int>& bombsToExplode) const;
    void checkBombCollision(GameEngine& gameContext, BombWeaponComponent& bombComp) const;
    void explodeBomb(GameEngine& gameContext, int bombId) const;

};