#include "MenuSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/WorldData.hpp>

#include <iostream>

MenuSystem::MenuSystem() {}

MenuSystem::~MenuSystem() {}

void MenuSystem::update(GameEngine& gameContext) const {
	MenuComponent&  menuComp    = gameContext.entityMan.getComponents<MenuComponent>()[ (size_t)0 ];
	InputComponent& playerInput = gameContext.entityMan.getComponent<InputComponent>(WorldData::playerId);

	std::size_t lastSelected = menuComp.selectedOption;

	if (playerInput.movingUp) {
		if (menuComp.selectedOption < menuComp.options.size() - 1) {
			++menuComp.selectedOption;
		}
		else {
			menuComp.selectedOption = 0;
		}
	}
	else if (playerInput.movingDown) {
		if (menuComp.selectedOption != 0) {
			--menuComp.selectedOption;
		}
		else {
			menuComp.selectedOption = menuComp.options.size() - 1;
		}
	}


	if (lastSelected != menuComp.selectedOption) {
		selectOption(gameContext, menuComp);		
	}

	if (playerInput.select) {
		acceptOption(gameContext, menuComp);
	}
}

void MenuSystem::selectOption(GameEngine& gameContext, MenuComponent& menuComp) const {
	
	switch (menuComp.options[menuComp.selectedOption])
	{
	case MenuOptions::BACK:
		std::cout << "Selecciono BACK\n";
		break;
	case MenuOptions::EXIT:
		std::cout << "Selecciono EXIT\n";
		break;
	case MenuOptions::OPTIONS:
		std::cout << "Selecciono OPTIONS\n";
		break;
	case MenuOptions::PLAY:
		std::cout << "Selecciono PLAY\n";
		break;
	}
}

void MenuSystem::acceptOption(GameEngine& gameContext, MenuComponent& menuComp) const {

	switch (menuComp.options[menuComp.selectedOption])
	{
	case MenuOptions::BACK:
		gameContext.setGameState(gameContext.getLastGameState());
		break;
	case MenuOptions::EXIT:
		gameContext.setPlaying(false);
		break;
	case MenuOptions::OPTIONS:
		std::cout << "Accedo a OPTIONS\n";
		break;
	case MenuOptions::PLAY:
		gameContext.setGameState(GameState::PLAYING);
		break;
	}
}