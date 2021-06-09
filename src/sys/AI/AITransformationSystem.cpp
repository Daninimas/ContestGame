#include "AITransformationSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>
#include <iostream>

AITransformationSystem::AITransformationSystem() {}

AITransformationSystem::~AITransformationSystem() {}


void AITransformationSystem::update(GameEngine& gameContext) const {
	auto& transformComponents = gameContext.entityMan->getComponents<AITransformationComponent>();
	std::vector<int> transformsCompToDelete;
	transformsCompToDelete.reserve(transformComponents.size());

	for (AITransformationComponent& transformComp : transformComponents) {
		if (gameContext.entityMan->existsComponent<SituationComponent>(transformComp.objectiveId) && hasToTransform(gameContext, transformComp)) {
			transform(gameContext, transformComp, transformsCompToDelete);
		}
	}

	for (int transformId : transformsCompToDelete) {
		gameContext.entityMan->eraseComponent<AITransformationComponent>(transformId);
	}
}


bool AITransformationSystem::hasToTransform(GameEngine& gameContext, AITransformationComponent& transformComp) const {
	bool result = false;

	// Check if has been hitted
	result = checkHitted(gameContext, gameContext.entityMan->getComponent<ColliderComponent>(transformComp.id));

	// Check if objective is in range
	if (!result) {
		result = checkRange(gameContext, transformComp);
	}

	return result;
}

bool AITransformationSystem::checkHitted(GameEngine& gameContext, ColliderComponent& transformEntColl) const {
	for (int hittingId : transformEntColl.boundingRoot.bounding.entitiesColliding) {
		if (gameContext.entityMan->getEntity(hittingId).getType() != EntityType::WALL) {
			return true;
		}
	}

	return false;
}

bool AITransformationSystem::checkRange(GameEngine & gameContext, AITransformationComponent & transformComp) const {
	SituationComponent& attackerSit = gameContext.entityMan->getComponent<SituationComponent>(transformComp.id);
	SituationComponent& objectiveSit = gameContext.entityMan->getComponent<SituationComponent>(transformComp.objectiveId);
	
	return Utils::objectiveInsideRange(attackerSit, objectiveSit, transformComp.range);
}




void AITransformationSystem::transform(GameEngine& gameContext, AITransformationComponent& transformComp, std::vector<int>& transformsCompToDelete) const {
	int entityId = transformComp.id;
	RenderComponent& renderComp = gameContext.entityMan->getComponent<RenderComponent>(entityId);
	SituationComponent& sitComp = gameContext.entityMan->getComponent<SituationComponent>(entityId);
	ColliderComponent& collComp = gameContext.entityMan->getComponent<ColliderComponent>(entityId);
	SensorComponent& sensorComp = gameContext.entityMan->getComponent<SensorComponent>(entityId);

	// Transform its appearance and atacks the objective
	sitComp.position.y -= (transformComp.newBoundingRoot.bounding.yDown - collComp.boundingRoot.bounding.yDown);

		// Collider
	collComp.boundingRoot = std::move(transformComp.newBoundingRoot);
		// render
	renderComp.sprite = std::move(transformComp.newSprite);
	renderComp.spriteRect = std::move(transformComp.newSpriteRect);
	renderComp.color = std::move(transformComp.newColor);
		// new situation
	sitComp.scale = transformComp.newScale;
		// move the sensor
	float halfX = (collComp.boundingRoot.bounding.xRight - collComp.boundingRoot.bounding.xLeft);
	float halfY = (collComp.boundingRoot.bounding.yDown - collComp.boundingRoot.bounding.yUp);
	sensorComp.sensorBounding = { sensorComp.sensorBounding.xLeft, halfX, sensorComp.sensorBounding.yUp, halfY-2.f };

		// Make it attack the objective
	AIChaseComponent& AIchaseComp = gameContext.entityMan->createComponent<AIChaseComponent>(entityId);
	AIMeleeAtkComponent& AImeleeComp = gameContext.entityMan->createComponent<AIMeleeAtkComponent>(entityId);
			// AIChaseComponent
	AIchaseComp.objectiveId = transformComp.objectiveId;
			// AIMeleeAtkComponent
	AImeleeComp.objectiveId = transformComp.objectiveId;
	

	// Delete this component
	transformsCompToDelete.push_back(entityId);

	// Update on render
	vector<int> aux;
	aux.push_back(entityId);
	gameContext.entityMan->addEntityToUpdate(entityId);
	gameContext.getWindowFacadeRef().updateTextures(gameContext, aux);
}
