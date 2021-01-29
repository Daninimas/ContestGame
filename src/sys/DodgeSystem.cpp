#include "DodgeSystem.hpp"

#include <eng/GameEngine.hpp>
#include <enum/EntityType.hpp>
#include <tools/AnimationManager.hpp>

#include <iostream>

DodgeSystem::DodgeSystem() {}

DodgeSystem::~DodgeSystem() {}

void DodgeSystem::update(GameEngine& gameContext) const {
	checkPlayerDodge(gameContext);

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
		velComp.velocity.x = velComp.speedX;
		velComp.velocity.x *= dodgeComp.velocityIncrementFactor;
		break;

	case DodgeComponent::Left:
		velComp.velocity.x = -velComp.speedX;
		velComp.velocity.x *= dodgeComp.velocityIncrementFactor;
		break;
	}

	// The cooldown resets only when if finish doing the dodge
	dodgeComp.cooldown = 0.f;
	dodgeComp.dodgeDuration += gameContext.getDeltaTime();

	// Set the animation to the entity
	AnimationComponent& animComp = gameContext.entityMan.getComponent<AnimationComponent>(dodgeComp.id);
	AnimationManager::setAnimationToEntity(gameContext, Animation::DODGE, animComp);
}


void DodgeSystem::checkPlayerDodge(GameEngine& gameContext) const {
	DodgeComponent& playerDodge = gameContext.entityMan.getComponent<DodgeComponent>(WorldElementsData::playerId);
	InputComponent& playerInput = gameContext.entityMan.getComponent<InputComponent>(WorldElementsData::playerId);

	playerDodge.timeFromLastMove += gameContext.getDeltaTime();

	if (playerInput.actualMovement == playerDodge.lastMovement && playerDodge.timeFromLastMove < playerDodge.dodgeTime && playerDodge.releasedKey) {
		playerDodge.activateDodge = true;
		playerDodge.dodgeDirection = playerInput.actualMovement;
	}

	if (playerInput.actualMovement != 0xFF) { //if key pressed
		playerDodge.timeFromLastMove = 0.f;
		playerDodge.lastMovement = playerInput.actualMovement;
		playerDodge.releasedKey = false;
	}
	else {
		playerDodge.releasedKey = true;
	}


	// Reset actual movement
	playerInput.actualMovement = 0xFF;
}