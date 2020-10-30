#pragma once

#include "../System.hpp"
#include <com/ComponentsIncluder>

class AITransformationSystem : public System {
public:
    AITransformationSystem();
    ~AITransformationSystem();

    void update(GameEngine& gameContext) const override;

private:
    bool hasToTransform(GameEngine& gameContext, AITransformationComponent& transformComp) const;
    bool checkHitted(GameEngine& gameContext, ColliderComponent& transformEntColl) const;
    bool checkRange(GameEngine& gameContext, AITransformationComponent& transformComp) const;


    void transform(GameEngine& gameContext, AITransformationComponent& transformComp, std::vector<int>& transformsCompToDelete) const;
};