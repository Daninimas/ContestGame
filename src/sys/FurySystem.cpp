#include "FurySystem.hpp"

#include <eng/GameEngine.hpp>

#include <iostream>
#include <vector>

FurySystem::FurySystem() {}

FurySystem::~FurySystem() {}

void FurySystem::update(GameEngine& gameContext) const {
	auto& furyComponents = gameContext.entityMan->getComponents<FuryComponent>();
	std::vector<int> furiesToDestroy;
	furiesToDestroy.reserve(furyComponents.size());

	for (FuryComponent& furyComp : furyComponents) {
		furyComp.lifeTime += gameContext.getDeltaTime();

		if (furyComp.lifeTime < furyComp.totalLifeTime) {
			applyFuryToTimers(gameContext, furyComp);

			setFuryColor(gameContext, furyComp);
		}
		else {
			// delete this fury (lifetime epended)
			furiesToDestroy.emplace_back(furyComp.id);
		}
	}


	for (int& id : furiesToDestroy) {
		gameContext.entityMan->eraseComponent<FuryComponent>(id);

		if (gameContext.entityMan->existsComponent<RenderComponent>(id)) {
			gameContext.getWindowFacadeRef().setColorToEntity(id, gameContext.entityMan->getComponent<RenderComponent>(id).color);
		}
	}
}


void FurySystem::applyFuryToTimers(GameEngine& gameContext, FuryComponent& furyComp) const {
	int furyId = furyComp.id;
	float incrementTime = (gameContext.getDeltaTime() * furyComp.timersSpeedIncFactor);

	if (gameContext.entityMan->existsComponent<MeleeWeaponComponent>(furyId)) {
		gameContext.entityMan->getComponent<MeleeWeaponComponent>(furyId).cooldown += incrementTime;
	}

	if (gameContext.entityMan->existsComponent<DistanceWeaponComponent>(furyId)) {
		gameContext.entityMan->getComponent<DistanceWeaponComponent>(furyId).cooldown += incrementTime;
	}


	if (gameContext.entityMan->existsComponent<VelocityComponent>(furyId)) {
		gameContext.entityMan->getComponent<VelocityComponent>(furyId).velocity.x *= furyComp.speedIncFactor;
	}
}


void FurySystem::setFuryColor(GameEngine& gameContext, FuryComponent& furyComp) const {
	gameContext.getWindowFacadeRef().setColorToEntity(furyComp.id, furyComp.furyColor);
}