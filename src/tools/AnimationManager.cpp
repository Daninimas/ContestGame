#include "AnimationManager.hpp"

#include <iostream>


void AnimationManager::setAnimationToEntity(GameEngine& gameContext, const Animation animation, AnimationComponent& animationComp) {
	if (animationComp.animation != animation) {
		GameObjectType entityGO = gameContext.entityMan->getEntity(animationComp.id).getGameObjectType();

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
	}
}


void AnimationManager::setRunningAnimation(GameEngine& gameContext, const Animation animation, AnimationComponent& animationComp, GameObjectType entityGO) {

	switch (entityGO)
	{
	case GameObjectType::PLAYER_GENERAL:
		animationComp.animation = animation;
		animationComp.framerate = 0.07f;
		animationComp.repeat = true;
		animationComp.startSpriteRect = { 100, 400, 60, 500 };
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
		animationComp.repeat = false;
		animationComp.startSpriteRect = { 100, 400, 60, 500 };
		animationComp.totalFrames = 1;
		animationComp.nextFrameAdvance = 512;
		break;

	case GameObjectType::MAINMENU:
		animationComp.animation = animation;
		animationComp.framerate = 0.5f;
		animationComp.repeat = true;
		animationComp.startSpriteRect = { 0, 800, 0, 600 };
		animationComp.totalFrames = 10;
		animationComp.nextFrameAdvance = 600;
		break;

	case GameObjectType::EXPLOSION:
		animationComp.animation = animation;
		animationComp.framerate = 0.01f;
		animationComp.repeat = false;
		animationComp.startSpriteRect = { 90, 160, 90, 180 };
		animationComp.totalFrames = 32;
		animationComp.nextFrameAdvance = 256;
		break;

	case GameObjectType::PLAYER_EXPLOSION:
		animationComp.animation = animation;
		animationComp.framerate = 0.01f;
		animationComp.repeat = false;
		animationComp.startSpriteRect = { 90, 160, 90, 180 };
		animationComp.totalFrames = 32;
		animationComp.nextFrameAdvance = 256;
		break;
	}
}


void AnimationManager::setDodgeAnimation(GameEngine& gameContext, const Animation animation, AnimationComponent& animationComp, GameObjectType entityGO) {

	switch (entityGO)
	{
	case GameObjectType::PLAYER_GENERAL:
		animationComp.animation = animation;
		animationComp.framerate = 0.1f;
		animationComp.repeat = true;
		animationComp.startSpriteRect = { 0, 512, 512, 1024 };
		animationComp.totalFrames = 4;
		animationComp.nextFrameAdvance = 512;
		break;
	}
}