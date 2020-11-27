#include "HUDSystem.hpp"
#include <eng/GameEngine.hpp>

#include <iostream>

HUDSystem::HUDSystem() {}

HUDSystem::~HUDSystem() {}

void HUDSystem::update(GameEngine& gameContext) const {
	updateTexts(gameContext);
}


void HUDSystem::updateTexts(GameEngine& gameContext) const {
	auto& textComponents = gameContext.entityMan.getComponents<TextComponent>();

	for (TextComponent& textComp : textComponents) {
		Entity& textEnt = gameContext.entityMan.getEntity(textComp.id);
		int playerId = WorldElementsData::playerId;

		switch (textEnt.getGameObjectType())
		{
		case GameObjectType::HUD_PLAYER_HEALTH:
			textComp.text = "Health: " + to_string(gameContext.entityMan.getComponent<HealthComponent>(playerId).currentHealth);
			gameContext.entityMan.addEntityToUpdate(textComp.id);
			break;

		case GameObjectType::HUD_PLAYER_AMMO:
			textComp.text = "Ammo: -";
			if (gameContext.entityMan.existsComponent<DistanceWeaponComponent>(playerId)){
				DistanceWeaponComponent& distComp = gameContext.entityMan.getComponent<DistanceWeaponComponent>(playerId);

				if (!distComp.infiniteAmmo) {
					textComp.text = "Ammo: " + to_string(distComp.ammo);
				}
			}
			break;
		}


		gameContext.entityMan.addEntityToUpdate(textComp.id);
	}
}
