#include "WaitAfterLoseLifeSystem.hpp"

#include <eng/GameEngine.hpp>

WaitAfterLoseLifeSystem::WaitAfterLoseLifeSystem() {}

WaitAfterLoseLifeSystem::~WaitAfterLoseLifeSystem() {}


void WaitAfterLoseLifeSystem::update(GameEngine& gameContext) const {
	WorldElementsData::waitLoseLifeTimeCounter += gameContext.getDeltaTime();

	if (WorldElementsData::waitLoseLifeTimeCounter > WorldElementsData::waitLoseLifeTime) {
		WorldElementsData::waitLoseLifeTimeCounter = 0.f; // Reset the counter again

		gameContext.popGameState(); // From WAIT_AFTER_LOSE_LIFE to PLAYING again
	}
}