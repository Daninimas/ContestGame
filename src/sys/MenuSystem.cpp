#include "MenuSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/WorldElementsData.hpp>

#include <iostream>

MenuSystem::MenuSystem() {}

MenuSystem::~MenuSystem() {}

void MenuSystem::update(GameEngine& gameContext) const {
	MenuComponent&  menuComp    = gameContext.entityMan.getComponents<MenuComponent>()[ (size_t)0 ];
	InputComponent& playerInput = gameContext.entityMan.getComponent<InputComponent>(WorldElementsData::playerId);

	std::size_t lastSelected = menuComp.selectedOption;

	if (playerInput.movingDown) {
		if (menuComp.selectedOption < menuComp.optionsId.size() - 1) {
			++menuComp.selectedOption;
		}
		else {
			menuComp.selectedOption = 0;
		}
	}
	else if (playerInput.movingUp) {
		if (menuComp.selectedOption != 0) {
			--menuComp.selectedOption;
		}
		else {
			menuComp.selectedOption = menuComp.optionsId.size() - 1;
		}
	}


	if (lastSelected != menuComp.selectedOption || menuComp.firstTime) {
		selectOption(gameContext, menuComp, lastSelected);		
	}

	if (playerInput.select) {
		acceptOption(gameContext, menuComp);
	}

	menuComp.firstTime = false;
}

void MenuSystem::selectOption(GameEngine& gameContext, MenuComponent& menuComp, std::size_t lastSelected) const {
	
	// Deselect last
	gameContext.entityMan.getComponent<MenuOptionComponent>(menuComp.optionsId[lastSelected]).active = false;
	gameContext.entityMan.getComponent<TextComponent>(menuComp.optionsId[lastSelected]).color = { 255, 255, 255, 255 };
	gameContext.entityMan.addEntityToUpdate(menuComp.optionsId[lastSelected]);
	// Select new selection
	gameContext.entityMan.getComponent<MenuOptionComponent>(menuComp.optionsId[menuComp.selectedOption]).active = true;
	gameContext.entityMan.getComponent<TextComponent>(menuComp.optionsId[menuComp.selectedOption]).color = { 255, 0, 0, 255 };
	gameContext.entityMan.addEntityToUpdate(menuComp.optionsId[menuComp.selectedOption]);

	/*switch (menuComp.optionsId[menuComp.selectedOption])
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
	}*/
}

void MenuSystem::acceptOption(GameEngine& gameContext, MenuComponent& menuComp) const {

	switch ( gameContext.entityMan.getComponent<MenuOptionComponent>(menuComp.optionsId[menuComp.selectedOption]).option )
	{
	case MenuOptions::BACK:
		gameContext.popGameState();
		break;
	case MenuOptions::EXIT:
		gameContext.setPlaying(false);
		break;
	case MenuOptions::PLAY:
		std::cout << "Jugando...\n";
		gameContext.pushGameState(GameState::PLAYING);
		break;
	case MenuOptions::CONTROLS:
		std::cout << "Accedo a CONTROLS\n";
		gameContext.pushGameState(GameState::CONTROLS);
		break;

		// Set controls (KEYS)
	case MenuOptions::SET_KEY_ATTACK:
		gameContext.entityMan.getComponent<InputComponent>(WorldElementsData::playerId).controlToChange = Controls::ACTION;
		gameContext.pushGameState(GameState::WAIT_INPUT);
		break;
	case MenuOptions::SET_KEY_JUMP:
		gameContext.entityMan.getComponent<InputComponent>(WorldElementsData::playerId).controlToChange = Controls::JUMP;
		gameContext.pushGameState(GameState::WAIT_INPUT);
		break;
	case MenuOptions::SET_KEY_LEFT:
		gameContext.entityMan.getComponent<InputComponent>(WorldElementsData::playerId).controlToChange = Controls::MOVE_LEFT;
		gameContext.pushGameState(GameState::WAIT_INPUT);
		break;
	case MenuOptions::SET_KEY_RIGHT:
		gameContext.entityMan.getComponent<InputComponent>(WorldElementsData::playerId).controlToChange = Controls::MOVE_RIGHT;
		gameContext.pushGameState(GameState::WAIT_INPUT);
		break;
	case MenuOptions::SET_KEY_UP:
		gameContext.entityMan.getComponent<InputComponent>(WorldElementsData::playerId).controlToChange = Controls::MOVE_UP;
		gameContext.pushGameState(GameState::WAIT_INPUT);
		break;
	case MenuOptions::SET_KEY_DOWN:
		gameContext.entityMan.getComponent<InputComponent>(WorldElementsData::playerId).controlToChange = Controls::MOVE_DOWN;
		gameContext.pushGameState(GameState::WAIT_INPUT);
		break;
	}

	gameContext.eraseEntityByID(menuComp.id);
}