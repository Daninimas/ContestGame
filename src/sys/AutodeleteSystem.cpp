#include "AutodeleteSystem.hpp"

#include <eng/GameEngine.hpp>
#include <vector>

AutodeleteSystem::AutodeleteSystem() {}

AutodeleteSystem::~AutodeleteSystem() {}


void AutodeleteSystem::update(GameEngine& gameContext) const {
	auto& autodeleteComponents = gameContext.entityMan.getComponents<AutodeleteComponent>();
	std::vector<int> entitiesToDelete {};

	entitiesToDelete.reserve(autodeleteComponents.size());

	for (AutodeleteComponent& autodeleteComp : autodeleteComponents) {
		autodeleteComp.timeToDeleteCounter += gameContext.getDeltaTime();

		if (autodeleteComp.timeToDeleteCounter > autodeleteComp.timeToDelete) {
			entitiesToDelete.emplace_back(autodeleteComp.id);
		}
	}


	// Now delete the entites that their time has ended
	for (int& id : entitiesToDelete) {
		gameContext.eraseEntityByID(id);
	}
}