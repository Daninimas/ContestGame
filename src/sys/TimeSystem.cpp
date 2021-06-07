#include "TimeSystem.hpp"
#include <eng/GameEngine.hpp>
#include <tools/WorldElementsData.hpp>


TimeSystem::TimeSystem() {}

TimeSystem::~TimeSystem() {}


void TimeSystem::update(GameEngine& gameContext) const {
	WorldElementsData::timeInPhase += gameContext.getDeltaTime();
}