#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class CheckpointSystem : public System {
public:
    CheckpointSystem();
    ~CheckpointSystem();

    void update(GameEngine& gameContext) const override;

private:
    void checkActivation(GameEngine& gameContext, CheckpointComponent& checkpoint) const;

};