#include "WorldSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>

#include <iostream>


WorldSystem::WorldSystem() {}

WorldSystem::~WorldSystem() {}


void WorldSystem::update(GameEngine& gameContext) const {
	// Reset previous entities sensored
	deleteEntitiesOutOfWorld(gameContext);
}

void WorldSystem::deleteEntitiesOutOfWorld(GameEngine& gameContext) const {
	auto& situations = gameContext.entityMan.getComponents<SituationComponent>();
	std::vector<int> entitiesToDelete{};
	BoundingBox& worldBounding = WorldData::phaseLimits;

	entitiesToDelete.reserve(situations.size());
	for (SituationComponent& situation : situations) {
		if (situation.position.x < worldBounding.xLeft || situation.position.x > worldBounding.xRight || situation.position.y < worldBounding.yUp || situation.position.y > worldBounding.yDown) {

			if (!situation.noWorldDelete) {
				entitiesToDelete.push_back(situation.id);
			}
			else {
				// TODO do something wih the player when its outside of world
			}

		}
	}

	// Delete the entities outside
	for (size_t i = 0; i < entitiesToDelete.size(); ++i)
	{
		gameContext.eraseEntityByID(entitiesToDelete[i]);
	}
}