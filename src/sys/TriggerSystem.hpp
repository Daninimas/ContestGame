#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class TriggerSystem : public System {
public:
    TriggerSystem();
    ~TriggerSystem();

    void update(GameEngine& gameContext) const override;

private:
    void activateFunction(GameEngine& gameContext, TriggerComponent& trigger) const;

    // ---------------------- TRIGGER FUNCTIONS
    void playMusic(GameEngine& gameContext, TriggerComponent& trigger) const;
    void playSoundEffect(GameEngine& gameContext, TriggerComponent& trigger) const;
    void createEntity(GameEngine& gameContext, TriggerComponent& trigger) const;

};