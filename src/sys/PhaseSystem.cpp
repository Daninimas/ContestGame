#include "PhaseSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/WorldElementsData.hpp>
#include <tools/Utils.hpp>
#include <tools/MapLoader.hpp>

#include <iostream>

PhaseSystem::PhaseSystem() {}

PhaseSystem::~PhaseSystem() {}

void PhaseSystem::update(GameEngine& gameContext) const {
	checkIfChangePhase(gameContext);
}

void PhaseSystem::checkIfChangePhase(GameEngine& gameContext) const {
	// activates the change of phase when the player is close to the limit and all enemies entities have been killed

	if (WorldElementsData::enemiesInWorld == 0) {
		SituationComponent& playerSit = gameContext.entityMan.getComponent<SituationComponent>(WorldElementsData::playerId);

		if (Utils::entityInPhaseLimit(gameContext, playerSit)) {
			WorldComponent& worldComp = gameContext.entityMan.getComponent<WorldComponent>(WorldElementsData::worldId);

			if (worldComp.currentPhaseNumber < worldComp.numberOfPhases) {
				changeToNextPhase(gameContext, worldComp);
			}
			else { // End of the world
				//changeToNextWorld(gameContext);
			}
		}
	}
}

void PhaseSystem::changeToNextPhase(GameEngine& gameContext, WorldComponent& worldComp) const {
	WorldPhase previousPhase = worldComp.currentPhase; // copy of the previous phase
	uint8_t previousPhaseDirection = worldComp.currentPhase.direction;

	++worldComp.currentPhaseNumber;
	MapLoader::loadMapPhase( gameContext, worldComp.worldPath, "Phase"+to_string(worldComp.currentPhaseNumber) );

	/*if (worldComp.currentPhase.phaseMusic.soundPath == previousPhase.phaseMusic.soundPath) {
		worldComp.currentPhase.phaseMusic.soundPath = std::move(previousPhase.phaseMusic.soundPath);
	}
	else {
		gameContext.getSoundFacadeRef().loadMusic(worldComp.currentPhase.phaseMusic.soundPath);
		gameContext.getSoundFacadeRef().playMusic(worldComp.currentPhase.phaseMusic);
	}*/

	Utils::setPhaseStartToView(gameContext, previousPhaseDirection);
}
