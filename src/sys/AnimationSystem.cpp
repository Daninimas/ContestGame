#include "AnimationSystem.hpp"

#include <eng/GameEngine.hpp>
#include <iostream>

AnimationSystem::AnimationSystem() {}

AnimationSystem::~AnimationSystem() {}


void AnimationSystem::update(GameEngine& gameContext) const {
	auto& animations = gameContext.entityMan.getComponents<AnimationComponent>();

	for (AnimationComponent& animComp : animations) {
		animComp.currentTime += gameContext.getDeltaTime();

		std::cout << "animComp.currentTime: " << animComp.currentTime << " animComp.framerate: " << animComp.framerate << "\n";
		if (animComp.currentTime > animComp.framerate && animComp.animation != Animation::NONE) {
			changeFrame(gameContext, animComp);
			std::cout << (int)animComp.animation << "\n";

		}
	}
}


void AnimationSystem::changeFrame(GameEngine& gameContext, AnimationComponent& animComp) const {
	RenderComponent& renderComp = gameContext.entityMan.getComponent<RenderComponent>(animComp.id);

	++animComp.actualFrame;

	if ( animComp.actualFrame >= animComp.totalFrames ) {
		if (!animComp.repeat) {
			return;
		}
		animComp.actualFrame = 0;
	}

	// Update spriteRect
	renderComp.spriteRect.xLeft = animComp.startSpriteRect.xLeft + ( animComp.nextFrameAdvance * animComp.actualFrame);
	renderComp.spriteRect.xRight = animComp.startSpriteRect.xRight + (animComp.nextFrameAdvance * animComp.actualFrame);
	renderComp.spriteRect.yUp = animComp.startSpriteRect.yUp;
	renderComp.spriteRect.yDown = animComp.startSpriteRect.yDown;

	gameContext.entityMan.addEntityToUpdate(animComp.id);
	animComp.currentTime = 0.f;
}
