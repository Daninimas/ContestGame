#include "AnimationSystem.hpp"

#include <eng/GameEngine.hpp>
#include <iostream>

AnimationSystem::AnimationSystem() {}

AnimationSystem::~AnimationSystem() {}


void AnimationSystem::update(GameEngine& gameContext) const {
	auto& animations = gameContext.entityMan->getComponents<AnimationComponent>();

	for (AnimationComponent& animComp : animations) {
		bool firstTime = false;
		
		// Check if animation has changed
		if (animComp.animation != animComp.lastAnimation) {
			// Reset component data
			animComp.actualFrame = -1;
			animComp.currentTime = animComp.framerate;
			animComp.ended = false;
			firstTime = true;
		}

		animComp.currentTime += gameContext.getDeltaTime();

		if (animComp.animation == Animation::SHOOT_UP) {
			float a = 0;
		}

		if (animComp.currentTime > animComp.framerate && animComp.animation != Animation::NONE) {
			if (animComp.totalFrames > 1 || animComp.actualFrame == -1) {
				changeFrame(gameContext, animComp);
			}
			else {
				animComp.ended = true;
			}

			firstTime = false;
		}

		// Set last animation
		animComp.lastAnimation = animComp.animation;
	}
}


void AnimationSystem::changeFrame(GameEngine& gameContext, AnimationComponent& animComp) const {
	RenderComponent& renderComp = gameContext.entityMan->getComponent<RenderComponent>(animComp.id);

	++animComp.actualFrame;

	if ( animComp.actualFrame >= animComp.totalFrames && animComp.totalFrames > 1 ) {
		animComp.ended = true;
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

	gameContext.entityMan->addEntityToUpdate(animComp.id);
	animComp.currentTime = 0.f;
}
