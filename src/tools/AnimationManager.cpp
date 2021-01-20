#include "AnimationManager.hpp"

#include <iostream>


void AnimationManager::setAnimationToEntity(GameEngine& gameContext, const Animation animation, AnimationComponent& animationComp) {
	if (animationComp.animation != animation) {
		GameObjectType entityGO = gameContext.entityMan.getEntity(animationComp.id).getGameObjectType();

		switch (animation)
		{
		case Animation::RUNNING:
			setRunningAnimation(gameContext, animation, animationComp, entityGO);
			break;

		case Animation::IDLE:
			setIdleAnimation(gameContext, animation, animationComp, entityGO);
			break;

		default:
			animationComp.animation = Animation::NONE;
			break;
		}

		// Reset component data
		animationComp.actualFrame = 0;
		animationComp.currentTime = 0.f;
	}
}


void AnimationManager::setRunningAnimation(GameEngine& gameContext, const Animation animation, AnimationComponent& animationComp, GameObjectType entityGO) {

	switch (entityGO)
	{
	case GameObjectType::PLAYER_GENERAL:
		animationComp.animation = animation;
		animationComp.framerate = 0.15f;
		animationComp.repeat = true;
		animationComp.startSpriteRect = { 0, 512, 0, 512 };
		animationComp.totalFrames = 9;
		animationComp.nextFrameAdvance = 512;
		break;
	}
}

void AnimationManager::setIdleAnimation(GameEngine& gameContext, const Animation animation, AnimationComponent& animationComp, GameObjectType entityGO) {

	switch (entityGO)
	{
	case GameObjectType::PLAYER_GENERAL:
		animationComp.animation = animation;
		animationComp.framerate = 0.2f;
		animationComp.repeat = true;
		animationComp.startSpriteRect = { 0, 512, 0, 512 };
		animationComp.totalFrames = 1;
		animationComp.nextFrameAdvance = 512;
		break;
	}
}
