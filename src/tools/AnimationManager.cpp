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

		case Animation::DODGE:
			setDodgeAnimation(gameContext, animation, animationComp, entityGO);
			break;

		default:
			animationComp.animation = Animation::NONE;
			break;
		}

		// Reset component data
		animationComp.actualFrame = 0;
		animationComp.currentTime = animationComp.framerate + 1;
	}
}


void AnimationManager::setRunningAnimation(GameEngine& gameContext, const Animation animation, AnimationComponent& animationComp, GameObjectType entityGO) {

	switch (entityGO)
	{
	case GameObjectType::PLAYER_GENERAL:
		animationComp.animation = animation;
		animationComp.framerate = 0.10f;
		animationComp.repeat = true;
		animationComp.startSpriteRect = { 0, 512, 0, 512 };
		animationComp.totalFrames = 12;
		animationComp.nextFrameAdvance = 512;
		break;
	}
}

void AnimationManager::setIdleAnimation(GameEngine& gameContext, const Animation animation, AnimationComponent& animationComp, GameObjectType entityGO) {

	switch (entityGO)
	{
	case GameObjectType::PLAYER_GENERAL:
		animationComp.animation = animation;
		animationComp.framerate = 1.f;
		animationComp.repeat = true;
		animationComp.startSpriteRect = { 0, 512, 0, 512 };
		animationComp.totalFrames = 1;
		animationComp.nextFrameAdvance = 512;
		break;
	}
}


void AnimationManager::setDodgeAnimation(GameEngine& gameContext, const Animation animation, AnimationComponent& animationComp, GameObjectType entityGO) {

	switch (entityGO)
	{
	case GameObjectType::PLAYER_GENERAL:
		std::cout << "hola\n";

		animationComp.animation = animation;
		animationComp.framerate = 0.1f;
		animationComp.repeat = false;
		animationComp.startSpriteRect = { 0, 512, 512, 1024 };
		animationComp.totalFrames = 4;
		animationComp.nextFrameAdvance = 512;
		break;
	}
}