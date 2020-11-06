#include "ShieldSystem.hpp"

#include <eng/GameEngine.hpp>
#include <iostream>


ShieldSystem::ShieldSystem() {}

ShieldSystem::~ShieldSystem() {}


void ShieldSystem::update(GameEngine& gameContext) const {
	auto& shieldComponents = gameContext.entityMan.getComponents<ShieldComponent>();

	for (ShieldComponent& shield : shieldComponents) {
		if (gameContext.entityMan.existsComponent<SituationComponent>(shield.objectiveId)) {
			setInObjectivePosition(gameContext, shield);
		}
	}
}


void ShieldSystem::setInObjectivePosition(GameEngine& gameContext, ShieldComponent& shield) const {
	SituationComponent& shieldSit    = gameContext.entityMan.getComponent<SituationComponent>(shield.id);
	SituationComponent& objectiveSit = gameContext.entityMan.getComponent<SituationComponent>(shield.objectiveId);


}
