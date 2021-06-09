#include "MenuSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/WorldElementsData.hpp>
#include <tools/Utils.hpp>

#include <iostream>

MenuSystem::MenuSystem() {}

MenuSystem::~MenuSystem() {}

void MenuSystem::update(GameEngine& gameContext) const {
	MenuComponent&  menuComp    = gameContext.entityMan->getComponents<MenuComponent>()[ (size_t)0 ];

	if (gameContext.entityMan->getEntity(menuComp.id).getGameObjectType() == GameObjectType::NEW_BEST_SCORE) { // Para el funcionamiento del menu de nueva mejor puntuacion
		updateNewBestScore(gameContext, menuComp);
		return;
	}

	InputComponent& playerInput = gameContext.entityMan->getComponent<InputComponent>(WorldElementsData::playerId);

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
	gameContext.entityMan->getComponent<MenuOptionComponent>(menuComp.optionsId[lastSelected]).active = false;
	gameContext.entityMan->getComponent<TextComponent>(menuComp.optionsId[lastSelected]).color = { 255, 255, 255, 255 };
	gameContext.entityMan->addEntityToUpdate(menuComp.optionsId[lastSelected]);
	// Select new selection
	gameContext.entityMan->getComponent<MenuOptionComponent>(menuComp.optionsId[menuComp.selectedOption]).active = true;
	gameContext.entityMan->getComponent<TextComponent>(menuComp.optionsId[menuComp.selectedOption]).color = { 255, 0, 0, 255 };
	gameContext.entityMan->addEntityToUpdate(menuComp.optionsId[menuComp.selectedOption]);
}

void MenuSystem::acceptOption(GameEngine& gameContext, MenuComponent& menuComp) const {

	switch ( gameContext.entityMan->getComponent<MenuOptionComponent>(menuComp.optionsId[menuComp.selectedOption]).option )
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

	case MenuOptions::MAIN_MENU:
		std::cout << "Vuelvo a MENU PRINCIPAL\n";
		gameContext.clearGameStateStack();
		gameContext.resetFlag = true;
		gameContext.pushGameState(GameState::PAUSE);
		break;

	case MenuOptions::TO_BEST_SCORES:
		if (gameContext.entityMan->getEntity(menuComp.id).getGameObjectType() == GameObjectType::NEW_BEST_SCORE)  // Para el funcionamiento del menu de nueva mejor puntuacion
			manageBestScoreAdding(gameContext, menuComp);

		std::cout << "Accedo a BEST SCORES\n";
		gameContext.pushGameState(GameState::BEST_SCORES);
		break;

	case MenuOptions::NEW_BEST_SCORE:
		//comprobamos que esta sea una nueva mejor puntuacion
		gameContext.entityMan->readBestScore();

		if (Utils::getNewPlayerScorePosition() < 0) {
			std::cout << "Accedo a BEST SCORES\n";
			gameContext.pushGameState(GameState::BEST_SCORES);
		}
		else {
			std::cout << "Accedo a NEW BEST SCORES\n";
			gameContext.pushGameState(GameState::NEW_BEST_SCORE);
		}
		break;

	case MenuOptions::SELECT_CHARACTER:
		return; // no hace nada
		break;


		// Set controls (KEYS)
	case MenuOptions::SET_KEY_ATTACK:
		gameContext.entityMan->getComponent<InputComponent>(WorldElementsData::playerId).controlToChange = Controls::ACTION;
		gameContext.pushGameState(GameState::WAIT_INPUT);
		break;
	case MenuOptions::SET_KEY_JUMP:
		gameContext.entityMan->getComponent<InputComponent>(WorldElementsData::playerId).controlToChange = Controls::JUMP;
		gameContext.pushGameState(GameState::WAIT_INPUT);
		break;
	case MenuOptions::SET_KEY_LEFT:
		gameContext.entityMan->getComponent<InputComponent>(WorldElementsData::playerId).controlToChange = Controls::MOVE_LEFT;
		gameContext.pushGameState(GameState::WAIT_INPUT);
		break;
	case MenuOptions::SET_KEY_RIGHT:
		gameContext.entityMan->getComponent<InputComponent>(WorldElementsData::playerId).controlToChange = Controls::MOVE_RIGHT;
		gameContext.pushGameState(GameState::WAIT_INPUT);
		break;
	case MenuOptions::SET_KEY_UP:
		gameContext.entityMan->getComponent<InputComponent>(WorldElementsData::playerId).controlToChange = Controls::MOVE_UP;
		gameContext.pushGameState(GameState::WAIT_INPUT);
		break;
	case MenuOptions::SET_KEY_DOWN:
		gameContext.entityMan->getComponent<InputComponent>(WorldElementsData::playerId).controlToChange = Controls::MOVE_DOWN;
		gameContext.pushGameState(GameState::WAIT_INPUT);
		break;
		// Set controls (JOYSTICK)
	case MenuOptions::SET_JOYSTICK_ATTACK:
		gameContext.entityMan->getComponent<InputComponent>(WorldElementsData::playerId).controlToChange = Controls::JOYSTICK_ACTION;
		gameContext.pushGameState(GameState::WAIT_INPUT);
		break;
	case MenuOptions::SET_JOYSTICK_JUMP:
		gameContext.entityMan->getComponent<InputComponent>(WorldElementsData::playerId).controlToChange = Controls::JOYSTICK_JUMP;
		gameContext.pushGameState(GameState::WAIT_INPUT);
		break;
	}

	gameContext.eraseEntityByID(menuComp.id);
}


void MenuSystem::updateNewBestScore(GameEngine& gameContext, MenuComponent& menuComp) const {
	InputComponent& playerInput = gameContext.entityMan->getComponent<InputComponent>(WorldElementsData::playerId);

	std::size_t lastSelected = menuComp.selectedOption;

	if (playerInput.movingRight) {
		if (menuComp.selectedOption < menuComp.optionsId.size() - 1) {
			++menuComp.selectedOption;
		}
		else {
			menuComp.selectedOption = 0;
		}
	}
	else if (playerInput.movingLeft) {
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

	// Para cambiar de letra
	if (playerInput.movingDown) {
		changeCharacter(gameContext, menuComp, +1);
	}
	if (playerInput.movingUp) {
		changeCharacter(gameContext, menuComp, -1);
	}

	menuComp.firstTime = false;
}

void MenuSystem::changeCharacter(GameEngine& gameContext, MenuComponent& menuComp, int step) const {
	MenuOptionComponent& optionComp = gameContext.entityMan->getComponent<MenuOptionComponent>(menuComp.optionsId[menuComp.selectedOption]);

	if (optionComp.option == MenuOptions::SELECT_CHARACTER) {

		if (step == +1) {
			if (optionComp.characterSelected < characters.size() - 1) {
				++optionComp.characterSelected;
			}
			else {
				optionComp.characterSelected = 0;
			}
		}
		else if (step == -1) {
			if (optionComp.characterSelected != 0) {
				--optionComp.characterSelected;
			}
			else {
				optionComp.characterSelected = characters.size() - 1;
			}
		}

		gameContext.entityMan->getComponent<TextComponent>(optionComp.id).text = characters[optionComp.characterSelected];
		gameContext.entityMan->addEntityToUpdate(optionComp.id);
	}
}

void MenuSystem::manageBestScoreAdding(GameEngine& gameContext, MenuComponent& menuComp) const {
	int scorePosition = Utils::getNewPlayerScorePosition();

	if (scorePosition != -1) {

		Score newScore; // First create the new score
		newScore.score = WorldElementsData::playerScore;
		newScore.name = gameContext.entityMan->getComponent<TextComponent>(menuComp.optionsId[0]).text + gameContext.entityMan->getComponent<TextComponent>(menuComp.optionsId[1]).text + gameContext.entityMan->getComponent<TextComponent>(menuComp.optionsId[2]).text;
		
		std::array<Score, 10> aux;
		int j = 0;
		auto& best_score_list = WorldElementsData::best_score_list;

		for (std::size_t i = 0; i < aux.size(); ++i) {
			if (scorePosition == i) {
				aux[i] = newScore;
			}
			else {
				aux[i] = best_score_list[j];
				++j;
			}
		}

		// Update file
		best_score_list.swap(aux);
		gameContext.entityMan->writeBestScore();
	}
}