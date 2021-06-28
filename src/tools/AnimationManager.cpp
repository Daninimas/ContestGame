#include "AnimationManager.hpp"

#include <iostream>


void AnimationManager::setAnimationToEntity(GameEngine& gameContext, const Animation animation, AnimationComponent& animationComp) {
	if (animationComp.animation != animation) {
		GameObjectType entityGO = gameContext.entityMan->getEntity(animationComp.id).getGameObjectType();

		switch (animation)
		{
		case Animation::RUNNING:
			if (animationComp.hasToEnd && !animationComp.ended) {

			}
			else {
				setRunningAnimation(gameContext, animation, animationComp, entityGO);
			}
			break;

		case Animation::IDLE:
			if (animationComp.hasToEnd && !animationComp.ended) {

			}
			else {
				setIdleAnimation(gameContext, animation, animationComp, entityGO);
			}
			break;

		case Animation::DODGE:
			setDodgeAnimation(gameContext, animation, animationComp, entityGO);
			break;

		case Animation::SHOOT_UP:
			if (animationComp.hasToEnd && !animationComp.ended) {

			}
			else {
				setShootUpAnimation(gameContext, animation, animationComp, entityGO);
			}
			break;

		case Animation::SHOOT_DOWN:
			if (animationComp.hasToEnd && !animationComp.ended) {

			}
			else {
				setShootDownAnimation(gameContext, animation, animationComp, entityGO);
			}
			break;

		case Animation::MELEE_ATTACK:
			setMeleeAttackAnimation(gameContext, animation, animationComp, entityGO);
			break;

		case Animation::TAKING_DAMAGE:
			setTakingDamageAnimation(gameContext, animation, animationComp, entityGO);
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
		animationComp.startSpriteRect = { 100, 418, 32, 472 };
		animationComp.totalFrames = 12;
		animationComp.nextFrameAdvance = 512;
		animationComp.hasToEnd = false;
		break;

	case GameObjectType::DRONE_FRIEND:
		animationComp.animation = animation;
		animationComp.framerate = 0.1f;
		animationComp.repeat = false;
		animationComp.startSpriteRect = { 8, 494, 609, 832 };
		animationComp.totalFrames = 1;
		animationComp.nextFrameAdvance = 512;
		animationComp.hasToEnd = false;
		break;
	}
}

void AnimationManager::setIdleAnimation(GameEngine& gameContext, const Animation animation, AnimationComponent& animationComp, GameObjectType entityGO) {

	switch (entityGO)
	{
	case GameObjectType::PLAYER_GENERAL:
		animationComp.animation = animation;
		animationComp.framerate = 0.01f;
		animationComp.repeat = false;
		animationComp.startSpriteRect = { 100, 418, 32, 472 };
		animationComp.totalFrames = 1;
		animationComp.nextFrameAdvance = 512;
		animationComp.hasToEnd = false;
		break;

	case GameObjectType::MAINMENU:
		animationComp.animation = animation;
		animationComp.framerate = 2.f;
		animationComp.repeat = true;
		animationComp.startSpriteRect = { 0, 800, 0, 600 };
		animationComp.totalFrames = 16;
		animationComp.nextFrameAdvance = 800;
		animationComp.hasToEnd = false;
		break;

	case GameObjectType::EXPLOSION:
		animationComp.animation = animation;
		animationComp.framerate = 0.01f;
		animationComp.repeat = false;
		animationComp.startSpriteRect = { 90, 160, 90, 180 };
		animationComp.totalFrames = 32;
		animationComp.nextFrameAdvance = 256;
		animationComp.hasToEnd = false;
		break;

	case GameObjectType::PLAYER_EXPLOSION:
		animationComp.animation = animation;
		animationComp.framerate = 0.01f;
		animationComp.repeat = false;
		animationComp.startSpriteRect = { 90, 160, 90, 180 };
		animationComp.totalFrames = 32;
		animationComp.nextFrameAdvance = 256;
		animationComp.hasToEnd = false;
		break;

	case GameObjectType::DRONE_FRIEND:
		animationComp.animation = animation;
		animationComp.framerate = 0.1f;
		animationComp.repeat = false;
		animationComp.startSpriteRect = { 8, 494, 133, 316 };
		animationComp.totalFrames = 1;
		animationComp.nextFrameAdvance = 512;
		animationComp.hasToEnd = false;
		break;
	}
}


void AnimationManager::setDodgeAnimation(GameEngine& gameContext, const Animation animation, AnimationComponent& animationComp, GameObjectType entityGO) {

	switch (entityGO)
	{
	case GameObjectType::PLAYER_GENERAL:
		animationComp.animation = animation;
		animationComp.framerate = 0.1f;
		animationComp.repeat = false;
		animationComp.startSpriteRect = { 100, 418, 1845, 2254 };
		animationComp.totalFrames = 4;
		animationComp.nextFrameAdvance = 512;
		animationComp.hasToEnd = false;
		break;
	}
}


void AnimationManager::setShootUpAnimation(GameEngine& gameContext, const Animation animation, AnimationComponent& animationComp, GameObjectType entityGO) {

	switch (entityGO)
	{
	case GameObjectType::PLAYER_GENERAL:
		animationComp.animation = animation;
		animationComp.framerate = 0.1f;
		animationComp.repeat = false;
		animationComp.startSpriteRect = { 100, 418, 1360, 1798 };
		animationComp.totalFrames = 1;
		animationComp.nextFrameAdvance = 512;
		animationComp.hasToEnd = false;
		break;
	}
}

void AnimationManager::setShootDownAnimation(GameEngine& gameContext, const Animation animation, AnimationComponent& animationComp, GameObjectType entityGO) {
	switch (entityGO)
	{
	case GameObjectType::PLAYER_GENERAL:
		animationComp.animation = animation;
		animationComp.framerate = 0.1f;
		animationComp.repeat = false;
		animationComp.startSpriteRect = { 100, 418, 988, 1345 };
		animationComp.totalFrames = 1;
		animationComp.nextFrameAdvance = 512;
		animationComp.hasToEnd = false;
		break;
	}
}


void AnimationManager::setMeleeAttackAnimation(GameEngine& gameContext, const Animation animation, AnimationComponent& animationComp, GameObjectType entityGO) {
	switch (entityGO)
	{
	case GameObjectType::PLAYER_GENERAL:
		animationComp.animation = animation;
		animationComp.framerate = 0.1f;
		animationComp.repeat = false;
		animationComp.startSpriteRect = { 100, 418, 484, 910 };
		animationComp.totalFrames = 3;
		animationComp.nextFrameAdvance = 512;
		animationComp.hasToEnd = true;
		break;
	}
}

void AnimationManager::setTakingDamageAnimation(GameEngine& gameContext, const Animation animation, AnimationComponent& animationComp, GameObjectType entityGO) {
	switch (entityGO)
	{
	case GameObjectType::PLAYER_GENERAL:
		animationComp.animation = animation;
		animationComp.framerate = 0.2f;
		animationComp.repeat = false;
		animationComp.startSpriteRect = { 100, 418, 2290, 2710 };
		animationComp.totalFrames = 1;
		animationComp.nextFrameAdvance = 512;
		animationComp.hasToEnd = true;
		animationComp.ended = false;
		break;
	}
}
