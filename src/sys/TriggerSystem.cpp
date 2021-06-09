#include "TriggerSystem.hpp"

#include <eng/GameEngine.hpp>
#include <enum/TriggerFunction.hpp>
#include <tools/Utils.hpp>

#include <iostream>


TriggerSystem::TriggerSystem() {}

TriggerSystem::~TriggerSystem() {}


void TriggerSystem::update(GameEngine& gameContext) const {
	auto& triggers = gameContext.entityMan->getComponents<TriggerComponent>();
	std::vector<int> triggersToDelete;
	triggersToDelete.reserve(triggers.size());

	for (TriggerComponent& trigger : triggers) {
		ColliderComponent& collComp = gameContext.entityMan->getComponent<ColliderComponent>(trigger.id);

		if (Utils::checkCollidingWithEntity(collComp.boundingRoot, WorldElementsData::playerId)) {
			activateFunction(gameContext, trigger);
			triggersToDelete.emplace_back(trigger.id);
		}
	}


	for (int id : triggersToDelete) {
		gameContext.eraseEntityByID(id);
	}
}


void TriggerSystem::activateFunction(GameEngine& gameContext, TriggerComponent& trigger) const {

	for (TriggerFunction& function : trigger.functions) {

		switch (function)
		{
		case TriggerFunction::PLAY_MUSIC:
			playMusic(gameContext, trigger);
			break;

		case TriggerFunction::PLAY_SOUND:
			playSoundEffect(gameContext, trigger);
			break;

		case TriggerFunction::CREATE_ENTITY:
			createEntity(gameContext, trigger);
			break;

		case TriggerFunction::GIVE_CHILD_POINTS:
			WorldElementsData::playerScore += -(gameContext.entityMan->getComponent<HealthComponent>(trigger.id).score);
			break;
		}
	}
}



// ---------------------- TRIGGER FUNCTIONS

void TriggerSystem::playMusic(GameEngine& gameContext, TriggerComponent& trigger) const {
	// Start music
	gameContext.getSoundFacadeRef().loadMusic(trigger.sound.soundPath);
	gameContext.getSoundFacadeRef().playMusic(trigger.sound);
}

void TriggerSystem::playSoundEffect(GameEngine& gameContext, TriggerComponent& trigger) const {
	// Start music
	gameContext.getSoundFacadeRef().loadSound(trigger.sound.soundPath);
	gameContext.getSoundFacadeRef().playSound(trigger.sound);
}

void TriggerSystem::createEntity(GameEngine& gameContext, TriggerComponent& trigger) const {
	// not functional yet
}