#pragma once

#include <eng/GameEngine.hpp>
#include <enum/AnimationEnum.hpp>

class AnimationManager
{
public:
	static void setAnimationToEntity(GameEngine& gameContext, const Animation animation, AnimationComponent& animationComp);  // Sets an animation to the entity


private:
	static void setRunningAnimation(GameEngine& gameContext, const Animation animation, AnimationComponent& animationComp, GameObjectType entityGO);
	static void setIdleAnimation(GameEngine& gameContext, const Animation animation, AnimationComponent& animationComp, GameObjectType entityGO);
	static void setDodgeAnimation(GameEngine& gameContext, const Animation animation, AnimationComponent& animationComp, GameObjectType entityGO);
	static void setShootUpAnimation(GameEngine& gameContext, const Animation animation, AnimationComponent& animationComp, GameObjectType entityGO);
	static void setShootDownAnimation(GameEngine& gameContext, const Animation animation, AnimationComponent& animationComp, GameObjectType entityGO);
	static void setMeleeAttackAnimation(GameEngine& gameContext, const Animation animation, AnimationComponent& animationComp, GameObjectType entityGO);
	static void setTakingDamageAnimation(GameEngine& gameContext, const Animation animation, AnimationComponent& animationComp, GameObjectType entityGO);
};

