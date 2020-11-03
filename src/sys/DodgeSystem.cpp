#include "DodgeSystem.hpp"

#include <eng/GameEngine.hpp>
#include <enum/EntityType.hpp>

#include <iostream>

DodgeSystem::DodgeSystem() {}

DodgeSystem::~DodgeSystem() {}

void DodgeSystem::update(GameEngine& gameContext) const {
	auto& dodgeComponents = gameContext.entityMan.getComponents<DodgeComponent>();

	for (DodgeComponent& dodgeComp : dodgeComponents) {
		dodgeComp.cooldown += gameContext.getDeltaTime();
		if (dodgeComp.dodgeDuration < dodgeComp.dodgeMaxDuration) {
			doDodge(gameContext, dodgeComp);
		}
		else if (dodgeComp.activateDodge && dodgeComp.cooldown > dodgeComp.maxCooldown && gameContext.entityMan.existsComponent<VelocityComponent>(dodgeComp.id)) {
			dodgeComp.dodgeDuration = 0.f;
		}

		dodgeComp.activateDodge = false;
	}
}

// el dodge tiene que durar un tiempo, ahora mismo, dura una iteración

void DodgeSystem::doDodge(GameEngine& gameContext, DodgeComponent& dodgeComp) const {
	VelocityComponent& velComp = gameContext.entityMan.getComponent<VelocityComponent>(dodgeComp.id);


	switch (dodgeComp.dodgeDirection) {
	case DodgeComponent::Right:
		velComp.velocityX = velComp.speedX;
		velComp.velocityX *= dodgeComp.velocityIncrementFactor;
		break;

	case DodgeComponent::Left:
		velComp.velocityX = -velComp.speedX;
		velComp.velocityX *= dodgeComp.velocityIncrementFactor;
		break;
	}

	// The cooldown resets only when if finish doing the dodge
	dodgeComp.cooldown = 0.f;
	dodgeComp.dodgeDuration += gameContext.getDeltaTime();
}