#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class AnimationSystem : public System {
public:
    AnimationSystem();
    ~AnimationSystem();

    void update(GameEngine& gameContext) const override;

private:
    void changeFrame(GameEngine& gameContext, AnimationComponent& animComp) const;

};