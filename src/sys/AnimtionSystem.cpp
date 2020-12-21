#include "AnimtionSystem.hpp"

#include <eng/GameEngine.hpp>
#include <iostream>

AnimationSystem::AnimationSystem() {}

AnimationSystem::~AnimationSystem() {}


void AnimationSystem::update(GameEngine& gameContext) const {
	auto& animations = gameContext.entityMan.getComponents<AnimationComponent>();

	for (AnimationComponent& animComp : animations) {
		animComp.currentTime += gameContext.getDeltaTime();

		if (animComp.currentTime > animComp.framerate) {
			changeFrame(gameContext, animComp);
		}
	}
}


void AnimationSystem::changeFrame(GameEngine& gameContext, AnimationComponent& animComp) const {
	RenderComponent& renderComp = gameContext.entityMan.getComponent<RenderComponent>(animComp.id);

	++animComp.actualFrame;

	if ( animComp.actualFrame >= animComp.frames.size() ) {
		if (!animComp.repeat) {
			return;
		}
		animComp.actualFrame = 0;
	}

	renderComp.sprite = animComp.frames[animComp.actualFrame];

	gameContext.entityMan.addEntityToUpdate(animComp.id);
}
