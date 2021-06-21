#include "GameEngine.hpp"

#include <random>
#include <time.h>
#include <cstdint>

#include <iostream>

#include <thread>

#include <tools/Utils.hpp>

#include <sys/SystemsIncluder>
#include <SFML/Window/Joystick.hpp>

const bool SHOW_TIMERS      = false;
const bool CHECK_SYSTEMS    = false;

GameEngine::GameEngine()
    : windowFacade(800, 600, false), soundFacade(), gameStateStack(), entityMan(std::make_unique<EntityManager>()){

    srand(time(NULL)); // initialize the random seed
    //init();
    systems.reserve(MAX_SYSTEMS);
    systemsLate.reserve(MAX_SYSTEMS);

    if(SHOW_TIMERS) {
        //TO SYSTEMS
        systemTimes.reserve(MAX_SYSTEMS);
        for(int i = 0; i < MAX_SYSTEMS; ++i) {
            systemTimes.emplace_back(new float[MAX_TIMES]);
        }
        //TO UPDATERS
        updateTimes.reserve(MAX_TIMES);
        soundTimes.reserve(MAX_TIMES);
        renderTimes.reserve(MAX_TIMES);
        allTimes.reserve(MAX_TIMES);
    }
}

GameEngine::~GameEngine() {}

void GameEngine::reset() {
    
    //ResetGame systems
    systems.clear();
    systemsLate.clear();

    
    // Delete all entities from engine
    windowFacade.eraseAllEntities();

    // Stop all music
    soundFacade.stopAllMusic();

    // Delete previous entity manager
    entityMan.reset();
    entityMan = std::make_unique<EntityManager>();

    // Reset world elements data
    WorldElementsData::worldId = 0;
    WorldElementsData::playerId = std::numeric_limits<int>::max();
    WorldElementsData::activeCameraId = 0;
    WorldElementsData::playerDroneId = std::numeric_limits<int>::max();
    WorldElementsData::worldDistanceWeapons.clear();
    WorldElementsData::worldMeleeWeapons.clear();
    WorldElementsData::enemiesInWorld = 0;
    WorldElementsData::playerScore = 0;
    WorldElementsData::currentWorld = 0;

    // Restart game
    init();

    resetFlag = false;
}

void GameEngine::init() {
    pushGameState(GameState::MAIN_MENU);

    StaticEntitiesSystem staticSystem{};
    staticSystem.init(*this);
}

void GameEngine::setPlayingSystems() {
    float DELTA_TO_UPDATE = 1.f / 60.f; // Ponemos el juego a 60 fps

    systems.emplace_back(std::make_unique<AutodeleteSystem>());         // 00
    systems.emplace_back(std::make_unique<SensorSystem>());             // 01
    systems.emplace_back(std::make_unique<AIDroneSystem>());            // 02
    systems.emplace_back(std::make_unique<AIMeleeSystem>());            // 03
    systems.emplace_back(std::make_unique<AIDistanceSystem>());         // 04
    systems.emplace_back(std::make_unique<AITransformationSystem>());   // 05
    systems.emplace_back(std::make_unique<InputSystem>());              // 06
    systems.emplace_back(std::make_unique<InputJoystickSystem>());      // 07
    systems.emplace_back(std::make_unique<OrbitalWeaponSystem>());      // 08
    systems.emplace_back(std::make_unique<TurretSystem>());             // 09
    systems.emplace_back(std::make_unique<AttackSystem>());             // 10
    systems.emplace_back(std::make_unique<PickWeaponsSystem>());        // 11
    systems.emplace_back(std::make_unique<AIChaseSystem>());            // 12
    systems.emplace_back(std::make_unique<AIFlyingChaseSystem>());      // 13
    systems.emplace_back(std::make_unique<AIPounceSystem>());           // 14
    systems.emplace_back(std::make_unique<BombSystem>());               // 15
    systems.emplace_back(std::make_unique<AIBombDropSystem>());         // 16
    systems.emplace_back(std::make_unique<DodgeSystem>());              // 17
    systems.emplace_back(std::make_unique<SpawnSystem>());              // 18
    systems.emplace_back(std::make_unique<PickPowerUpSystem>());        // 19
    systems.emplace_back(std::make_unique<FurySystem>());               // 20
    systems.emplace_back(std::make_unique<PhaseSystem>());              // 21
    systems.emplace_back(std::make_unique<TriggerSystem>());            // 22
    systems.emplace_back(std::make_unique<CheckpointSystem>());         // 23
                                                                        
                                                                        
    systemsLate.emplace_back(std::make_unique<ShieldSystem>());         // 00
    systemsLate.emplace_back(std::make_unique<CollisionSystem>());      // 01            // Collision 2 veces, esto es lo mejor para que todo funcione, pero sera mejor hacer lo de los hilos para resolver las colisiones
    systemsLate.emplace_back(std::make_unique<PhysicsSystem>());        // 02
    systemsLate.emplace_back(std::make_unique<CollisionSystem>());      // 03
    systemsLate.emplace_back(std::make_unique<AnimationSystem>());      // 04
    systemsLate.emplace_back(std::make_unique<CameraSystem>());         // 05
    systemsLate.emplace_back(std::make_unique<HealthSystem>());         // 06
    systemsLate.emplace_back(std::make_unique<DeathSystem>());          // 07
    systemsLate.emplace_back(std::make_unique<HUDSystem>());            // 08
    systemsLate.emplace_back(std::make_unique<WorldSystem>());          // 09
    systemsLate.emplace_back(std::make_unique<TimeSystem>());           // 10
}

void GameEngine::setMenuSystems(GameObjectType const menu) {
    float DELTA_TO_UPDATE = 1.f / 20.f; // Ponemos el juego a 20 FPS

    std::cout << "EN PAUSA...\n";
    int menuId = entityMan->createMenu(*this, menu);

    systems.emplace_back(std::make_unique<InputSystem>());
    systems.emplace_back(std::make_unique<InputJoystickSystem>());
    systems.emplace_back(std::make_unique<MenuSystem>());

    if (menu == GameObjectType::MAINMENU) {
        MenuComponent menuComp = entityMan->getComponent<MenuComponent>(menuId);
        getSoundFacadeRef().loadMusic(menuComp.menuMusic.soundPath);
        getSoundFacadeRef().playMusic(menuComp.menuMusic);

        systems.emplace_back(std::make_unique<AnimationSystem>());
    }
}


void GameEngine::run() {
    // For the interpolation
    std::chrono::time_point<std::chrono::system_clock> then;
    then = std::chrono::system_clock::now();

    while (playing) {
        if (resetFlag)
            reset();

        if (gameStateChanged) {
            // State recently changed. Systems vector must be updated
            systems.clear();
            systemsLate.clear();

            switch (getGameState()) {
            case GameState::GAMEOVER:
                // Pause playing music
                //soundFacade.pauseAllMusic();

                setMenuSystems(GameObjectType::GAME_OVER_MENU);
                break;

            case GameState::PLAYING:
                //soundFacade.playAllMusic();
                setPlayingSystems();
                break;

            case GameState::PAUSE:
                // Pause playing music
                //soundFacade.pauseAllMusic();

                setMenuSystems(GameObjectType::PAUSE);
                break;

            case GameState::CONTROLS:
                if (sf::Joystick::isConnected(0)) {
                    setMenuSystems(GameObjectType::CONTROLS_JOYSTICK);
                }
                else {
                    setMenuSystems(GameObjectType::CONTROLS_KEYBOARD);
                }
                break;

            case GameState::BEST_SCORES:
                setMenuSystems(GameObjectType::BEST_SCORES);
                break;

            case GameState::NEW_BEST_SCORE:
                setMenuSystems(GameObjectType::NEW_BEST_SCORE);
                break;

            case GameState::MAIN_MENU:
                setMenuSystems(GameObjectType::MAINMENU);
                break;

            case GameState::WAIT_INPUT:
                break;

            case GameState::WAIT_AFTER_LOSE_LIFE:
                systems.emplace_back(std::make_unique<WaitAfterLoseLifeSystem>());
                systemsLate.emplace_back(std::make_unique<CameraSystem>());
                systemsLate.emplace_back(std::make_unique<WorldSystem>());
                break;

            case GameState::NEXT_LEVEL:
                loadNextLevel();
                break;
            }

            gameStateChanged = false;
        }

        calculateDeltaTime(then);


        if(SHOW_TIMERS) {

            deltaFromLastUpdate += deltaTime;
            if (deltaFromLastUpdate >= DELTA_TO_UPDATE) {
                manageFPS();

                std::chrono::time_point<std::chrono::system_clock> allChronoInit;
                allChronoInit = std::chrono::system_clock::now();


                std::chrono::time_point<std::chrono::system_clock> updateChronoInit;
                updateChronoInit = std::chrono::system_clock::now();
                updateWithTimers();
                std::chrono::time_point<std::chrono::system_clock> updateChronoFin;
                updateChronoFin = std::chrono::system_clock::now();

                std::chrono::time_point<std::chrono::system_clock> soundChronoInit;
                soundChronoInit = std::chrono::system_clock::now();
                updateSound();
                std::chrono::time_point<std::chrono::system_clock> soundChronoFin;
                soundChronoFin = std::chrono::system_clock::now();

                std::chrono::time_point<std::chrono::system_clock> renderChronoInit;
                renderChronoInit = std::chrono::system_clock::now();
                render();
                std::chrono::time_point<std::chrono::system_clock> renderChronoFin;
                renderChronoFin = std::chrono::system_clock::now();


                std::chrono::time_point<std::chrono::system_clock> allChronoFin;
                allChronoFin = std::chrono::system_clock::now();

                std::chrono::duration<float, std::milli> systemSpent    = updateChronoFin   - updateChronoInit;
                std::chrono::duration<float, std::milli> soundSpent     = soundChronoFin    - soundChronoInit;
                std::chrono::duration<float, std::milli> renderSpent    = renderChronoFin   - renderChronoInit;
                std::chrono::duration<float, std::milli> allSpent       = allChronoFin      - allChronoInit;


                updateTimes.emplace_back(systemSpent.count());
                soundTimes.emplace_back(soundSpent.count());
                renderTimes.emplace_back(renderSpent.count());
                allTimes.emplace_back(allSpent.count());


                if(timesUpdated_upd == MAX_TIMES - 1) {
                    // get the average of the system
                    float totalUpdate   = 0.f;
                    float totalSound    = 0.f;
                    float totalRender   = 0.f;
                    float all           = 0.f;
                    for(int x = 0; x < MAX_TIMES; ++x) {
                        totalUpdate += updateTimes[x];
                        totalSound  += soundTimes[x];
                        totalRender += renderTimes[x];
                        all         += allTimes[x];
                    }


                    cout << "\nSystems" << " has spent: " << (totalUpdate / MAX_TIMES);
                    cout << "\nSound  " << " has spent: " << (totalSound / MAX_TIMES);
                    cout << "\nRender " << " has spent: " << (totalRender / MAX_TIMES);
                    cout << "\nTOTAL  " << " has spent: " << (all / MAX_TIMES);
                    //cout << "\nNum Render Nodes  : " << windowFacade.countRenderNodes();
                    cout << "\nNum Total Entities: " << entityMan->getEntities().size() << "\n\n\n";


                    //RESET
                    timesUpdated_upd = 0;
                    updateTimes.clear();
                    soundTimes.clear();
                    renderTimes.clear();
                    allTimes.clear();
                } else {
                    ++timesUpdated_upd;

                }

                deltaFromLastUpdate = 0;
            }//DELTA


        } else {
            deltaFromLastUpdate += deltaTime;
            if (deltaFromLastUpdate >= DELTA_TO_UPDATE) {
                manageFPS();
                update();
                updateSound();
                deltaFromLastUpdate = 0;
            }

            render();

            //std::this_thread::sleep_for(500ms);

        }
    }//PLAYING

    //GAME CLOSED
}

void GameEngine::updateWithTimers() {
    entityMan->clearEntitiesToUpdate();
    float totalTimeSpent = 0.f;

    for (size_t i = 0; i < systems.size(); ++i) {
        std::chrono::time_point<std::chrono::system_clock> thenS;
        thenS = std::chrono::system_clock::now();

        systems[i]->update(*this);

        std::chrono::time_point<std::chrono::system_clock> nowS;
        nowS = std::chrono::system_clock::now();
        std::chrono::duration<float, std::milli> elapsed_time = nowS - thenS;
        systemTimes.at(i)[timesUpdated_sys] = elapsed_time.count();
        if(timesUpdated_sys == MAX_TIMES - 1) {
            // get the average of the system
            float total = 0.f;
            for(int x = 0; x < MAX_TIMES; ++x) {
                total += systemTimes.at(i)[x];
            }
            cout << "The system: " << (int)i << " has spent: " << total / MAX_TIMES << endl;
        }
    }

    for (size_t i = 0; i < systemsLate.size(); ++i) {
        std::chrono::time_point<std::chrono::system_clock> thenS;
        thenS = std::chrono::system_clock::now();

        systemsLate[i]->update(*this);

        std::chrono::time_point<std::chrono::system_clock> nowS;
        nowS = std::chrono::system_clock::now();
        std::chrono::duration<float, std::milli> elapsed_time = nowS - thenS;
        systemTimes.at(i + systems.size())[timesUpdated_sys] = elapsed_time.count();
        if (timesUpdated_sys == MAX_TIMES - 1) {
            // get the average of the system
            float total = 0.f;
            for (int x = 0; x < MAX_TIMES; ++x) {
                total += systemTimes.at(i + systems.size())[x];
            }
            cout << "The system: " << (int)(i + systems.size()) << " has spent: " << total / MAX_TIMES << endl;
        }
    }

    if(timesUpdated_sys == MAX_TIMES - 1)
        timesUpdated_sys = 0;
    else
        ++timesUpdated_sys;

    updateEntitiesInWindow();
}

void GameEngine::update() {

    // Reset entities to update
    entityMan->clearEntitiesToUpdate();

    for (size_t i = 0; i < systems.size(); ++i) {

        if (CHECK_SYSTEMS)
            cout << "System: " << (int)i << "... " << endl;
        systems[i]->update(*this);
    }

    for (size_t i = 0; i < systemsLate.size(); ++i) {

        if (CHECK_SYSTEMS)
            cout << "System Late update: " << (int)i << "... " << endl;
        systemsLate[i]->update(*this);
    }

    updateEntitiesInWindow();
}

void GameEngine::updateSound() {
    //soundFacade.update(*this);
}

void GameEngine::render() {
    windowFacade.render(*this);
}


void GameEngine::updateEntitiesInWindow() {
    // Update situation in engine
    windowFacade.updateEntities(*this, entityMan->getEntitiesToUpdate());
}

void GameEngine::setPlaying(const bool p) {
    playing = p;
}

const float GameEngine::getDeltaTime() const {
    return deltaFromLastUpdate;
}

void GameEngine::calculateDeltaTime(std::chrono::time_point<std::chrono::system_clock> &then) {
    //calculate deltaTime
    std::chrono::time_point<std::chrono::system_clock> now;
    now = std::chrono::system_clock::now();
    std::chrono::duration<float, std::ratio<1>> elapsed_time = now - then;
    deltaTime = elapsed_time.count();

    if (deltaTime > 1/15.f) // el juego queda capado a estar a mas de 15fps
        deltaTime = 1/15.f;

    then = now;

    // FPS counter
    //int fps   = 1 / elapsed_time.count();
    //std::cout << "FPS: " << fps << std::endl;    
}

void GameEngine::manageFPS() {
    // New FPS counter
    frameTimeCounter += deltaFromLastUpdate;
    if (frameTimeCounter >= 1.f) { // When a second has passed
        windowFacade.updateFPSTextNode(framesPassed);
        framesPassed = 0;
        frameTimeCounter = 0.f;
    }

    ++framesPassed;
}

WindowFacade &GameEngine::getWindowFacadeRef() {
    return windowFacade;
}

SoundFacade &GameEngine::getSoundFacadeRef() {
    return soundFacade;
}



Entity &GameEngine::getEntity(int id) {
    return entityMan->getEntity(id);
}



void GameEngine::eraseEntityByID(int id) {
    if (entityMan->existEntity(id)) {

        EntityType entityType = entityMan->getEntity(id).getType();
        GameObjectType gameObjectType = entityMan->getEntity(id).getGameObjectType();

        entityMan->removeEntityToUpdate(id);
        windowFacade.eraseEntity(id);

        // Remove the render sons of the entity
        if (entityMan->existsComponent<SituationComponent>(id)) {
            auto& renderSons = entityMan->getComponent<SituationComponent>(id).sons;
            for (int son : renderSons) {
                eraseEntityByID(son);
            }
        }

        // Removing the entity ID on Components
        if (entityMan->existsComponent<ColliderComponent>(id)) {
            auto& colliders = entityMan->getComponents<ColliderComponent>();
            for (ColliderComponent& collider : colliders) {
                Utils::deleteCollidingWithObjective(collider.boundingRoot, id); // deletes the id from the colliding entities
            }
        }

        if (entityMan->existsComponent<SensorComponent>(id)) {
            auto& sensors = entityMan->getComponents<SensorComponent>();
            for (SensorComponent& sensor : sensors) {
                sensor.entitiesSensoring.erase(std::remove(sensor.entitiesSensoring.begin(), sensor.entitiesSensoring.end(), id), sensor.entitiesSensoring.end());
            }
        }

        if (entityMan->existsComponent<SpawnerComponent>(id)) {
            auto& spawners = entityMan->getComponents<SpawnerComponent>();
            for (SpawnerComponent& spawnComp : spawners) {
                spawnComp.spawnedObjsAlive.erase(std::remove(spawnComp.spawnedObjsAlive.begin(), spawnComp.spawnedObjsAlive.end(), id), spawnComp.spawnedObjsAlive.end());
            }
        }

        // Erase all options from the Menu Component
        if (entityMan->existsComponent<MenuComponent>(id)) {
            MenuComponent& menuComp = entityMan->getComponent<MenuComponent>(id);
            std::vector<int>& menuOptions = entityMan->getComponent<MenuComponent>(id).optionsId;

            for (size_t i = 0; i < menuOptions.size(); ++i) {
                eraseEntityByID(menuOptions[i]);
            }

            for (size_t i = 0; i < menuComp.textsId.size(); ++i) {
                eraseEntityByID(menuComp.textsId[i]);
            }
        }
        //getSoundFacadeRef().setParameterEventByID(id, STOP_SOUND);

        // Erase canon and text on turret (NUNCA DESTRUIR UNA TORRETA A MANO)
        if (entityMan->existsComponent<TurretComponent>(id)) {
            eraseEntityByID(entityMan->getComponent<TurretComponent>(id).turretGunID);
            eraseEntityByID(entityMan->getComponent<TurretComponent>(id).textID);

            TurretComponent& turretComp = entityMan->getComponent<TurretComponent>(id);
            if (turretComp.inUse) {
                entityMan->getComponent<InputComponent>(turretComp.userID).usingTurret = false;
            }
        }

        // Substract from world weapons
        for (int idWeapon : WorldElementsData::worldMeleeWeapons) {
            if (idWeapon == id) {
                WorldElementsData::worldMeleeWeapons.erase(std::remove(WorldElementsData::worldMeleeWeapons.begin(), WorldElementsData::worldMeleeWeapons.end(), id), WorldElementsData::worldMeleeWeapons.end());
            }
        }
        for (int idWeapon : WorldElementsData::worldDistanceWeapons) {
            if (idWeapon == id) {
                WorldElementsData::worldDistanceWeapons.erase(std::remove(WorldElementsData::worldDistanceWeapons.begin(), WorldElementsData::worldDistanceWeapons.end(), id), WorldElementsData::worldDistanceWeapons.end());
            }
        }

        // Subtract enemy from world 
        if (entityType == EntityType::ENEMY || entityType == EntityType::SPAWNER || gameObjectType == GameObjectType::DRONE_ENEMY) {
            --WorldElementsData::enemiesInWorld;
        }
        else if (gameObjectType == GameObjectType::DRONE_FRIEND) {
            WorldElementsData::playerDroneId = std::numeric_limits<int>::max();
        }

        entityMan->eraseEntityByID(id);
    }
}



std::unordered_map<int, Entity> &GameEngine::getEntities() {

    return entityMan->getEntities();
}

/*
std::vector<int> &GameEngine::getEntitiesToUpdate() {
    return entityMan->getEntitiesToUpdate();
}

void GameEngine::addEntityToUpdate(const int id) {
    entityMan->addEntityToUpdate(id);
}

void GameEngine::clearEntitiesToUpdate() {
    entityMan->clearEntitiesToUpdate();
}*/

GameState GameEngine::getGameState() const {
    return gameStateStack.top();
}

void GameEngine::popGameState() {
    gameStateStack.pop();
    gameStateChanged = true;
}

void GameEngine::pushGameState(const GameState gs) {
    if (gameStateStack.size() == 0 || gs != gameStateStack.top()) {
        gameStateStack.push(gs);
        gameStateChanged = true;
    }
}

void GameEngine::clearGameStateStack() {
    stack<GameState>().swap(gameStateStack);
    gameStateChanged = true;
}



void GameEngine::loadNextLevel() {
    // Delete all entities from engine
    windowFacade.eraseAllEntities();

    // Stop all music
    soundFacade.stopAllMusic();

    // Delete previous entity manager
    entityMan.reset();
    entityMan = std::make_unique<EntityManager>();

    // Reset world elements data
    WorldElementsData::worldId = 0;
    WorldElementsData::playerId = std::numeric_limits<int>::max();
    WorldElementsData::activeCameraId = 0;
    WorldElementsData::playerDroneId = std::numeric_limits<int>::max();
    WorldElementsData::worldDistanceWeapons.clear();
    WorldElementsData::worldMeleeWeapons.clear();
    WorldElementsData::enemiesInWorld = 0;
    ++WorldElementsData::currentWorld; // poner siguiente mapa

    StaticEntitiesSystem staticSystem{};
    staticSystem.init(*this);

    popGameState(); // Set playing again
    setPlayingSystems();
}