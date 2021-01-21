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
const float DELTA_TO_UPDATE = 1.f / 60.f;

GameEngine::GameEngine()
    : windowFacade(800, 600, false), soundFacade(), gameStateStack(){

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

    //ResetGame
    systems.clear();
    systemsLate.clear();
    init();

}

void GameEngine::init() {
    pushGameState(GameState::PAUSE);

    StaticEntitiesSystem staticSystem{};
    staticSystem.init(*this);

    //setPlayingSystems();
}

void GameEngine::setPlayingSystems() {
    systems.emplace_back(std::make_unique<SensorSystem>());
    systems.emplace_back(std::make_unique<AIMeleeSystem>());
    systems.emplace_back(std::make_unique<AIDistanceSystem>());
    systems.emplace_back(std::make_unique<AITransformationSystem>());
    systems.emplace_back(std::make_unique<InputSystem>());
    systems.emplace_back(std::make_unique<InputJoystickSystem>());
    systems.emplace_back(std::make_unique<AttackSystem>());
    systems.emplace_back(std::make_unique<PickWeaponsSystem>());
    systems.emplace_back(std::make_unique<AIChaseSystem>());
    systems.emplace_back(std::make_unique<AIFlyingChaseSystem>());
    systems.emplace_back(std::make_unique<AIPounceSystem>());
    systems.emplace_back(std::make_unique<BombSystem>());
    systems.emplace_back(std::make_unique<AIBombDropSystem>());
    systems.emplace_back(std::make_unique<DodgeSystem>());
    systems.emplace_back(std::make_unique<SpawnSystem>());
    systems.emplace_back(std::make_unique<PickPowerUpSystem>());
    systems.emplace_back(std::make_unique<FurySystem>());
    systems.emplace_back(std::make_unique<PhaseSystem>());
    systems.emplace_back(std::make_unique<TriggerSystem>());

    
    systemsLate.emplace_back(std::make_unique<ShieldSystem>());
    systemsLate.emplace_back(std::make_unique<CollisionSystem>()); // Collision 2 veces, esto es lo mejor para que todo funcione, pero sera mejor hacer lo de los hilos para resolver las colisiones
    systemsLate.emplace_back(std::make_unique<PhysicsSystem>());
    systemsLate.emplace_back(std::make_unique<CollisionSystem>());
    systemsLate.emplace_back(std::make_unique<AnimationSystem>());
    systemsLate.emplace_back(std::make_unique<CameraSystem>());
    systemsLate.emplace_back(std::make_unique<HealthSystem>());
    systemsLate.emplace_back(std::make_unique<DeathSystem>());
    systemsLate.emplace_back(std::make_unique<HUDSystem>());
    systemsLate.emplace_back(std::make_unique<WorldSystem>());
}

void GameEngine::setMenuSystems(GameObjectType const menu) {
    std::cout << "EN PAUSA...\n";
    entityMan.createMenu(*this, menu);

    systems.emplace_back(std::make_unique<InputSystem>());
    systems.emplace_back(std::make_unique<InputJoystickSystem>());
    systems.emplace_back(std::make_unique<MenuSystem>());
}


void GameEngine::run() {
    // For the interpolation
    std::chrono::time_point<std::chrono::system_clock> then;
    then = std::chrono::system_clock::now();

    while (playing) {

        if (gameStateChanged) {
            // State recently changed. Systems vector must be updated
            systems.clear();
            systemsLate.clear();

            switch (getGameState()) {
            case GameState::GAMEOVER:
                setMenuSystems(GameObjectType::GAME_OVER_MENU);
                break;

            case GameState::PLAYING:
                setPlayingSystems();
                break;

            case GameState::PAUSE:
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

            case GameState::WAIT_INPUT:
                break;

            case GameState::WAIT_AFTER_LOSE_LIFE:
                systems.emplace_back(std::make_unique<WaitAfterLoseLifeSystem>());
                systemsLate.emplace_back(std::make_unique<CameraSystem>());
                systemsLate.emplace_back(std::make_unique<WorldSystem>());
                break;
            }

            gameStateChanged = false;
        }

        calculateDeltaTime(then);


        if(SHOW_TIMERS) {

            deltaFromLastUpdate += deltaTime;
            if (deltaFromLastUpdate >= DELTA_TO_UPDATE) {


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
                    cout << "\nNum Total Entities: " << entityMan.getEntities().size() << "\n\n\n";


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
    entityMan.clearEntitiesToUpdate();
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


    if(timesUpdated_sys == MAX_TIMES - 1)
        timesUpdated_sys = 0;
    else
        ++timesUpdated_sys;


}

void GameEngine::update() {

    entityMan.clearEntitiesToUpdate();

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
    windowFacade.updateEntities(*this, entityMan.getEntitiesToUpdate());

    // Reset entities to update
    entityMan.clearEntitiesToUpdate();
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

WindowFacade &GameEngine::getWindowFacadeRef() {
    return windowFacade;
}

SoundFacade &GameEngine::getSoundFacadeRef() {
    return soundFacade;
}



Entity &GameEngine::getEntity(int id) {
    return entityMan.getEntity(id);
}



void GameEngine::eraseEntityByID(int id) {
    entityMan.removeEntityToUpdate(id);
    windowFacade.eraseEntity(id);


    // Removing the entity ID on Components
    auto& colliders = entityMan.getComponents<ColliderComponent>();
    for (ColliderComponent& collider : colliders) {
        Utils::deleteCollidingWithObjective(collider.boundingRoot, id); // deletes the id from the colliding entities
    }

    auto& sensors = entityMan.getComponents<SensorComponent>();
    for (SensorComponent& sensor : sensors) {
        sensor.entitiesSensoring.erase(std::remove(sensor.entitiesSensoring.begin(), sensor.entitiesSensoring.end(), id), sensor.entitiesSensoring.end());
    }

    auto& spawners = entityMan.getComponents<SpawnerComponent>();
    for (SpawnerComponent& spawnComp : spawners) {
        spawnComp.spawnedObjsAlive.erase(std::remove(spawnComp.spawnedObjsAlive.begin(), spawnComp.spawnedObjsAlive.end(), id), spawnComp.spawnedObjsAlive.end());
    }

    // Erase all options from the Menu Component
    if (entityMan.existsComponent<MenuComponent>(id)) {
        std::vector<int>& menuOptions = entityMan.getComponent<MenuComponent>(id).optionsId;

        for (size_t i = 0; i < menuOptions.size(); ++i) {
            eraseEntityByID(menuOptions[i]);
        }
    }
    //getSoundFacadeRef().setParameterEventByID(id, STOP_SOUND);

    // Subtract enemy from world 
    if (entityMan.getEntity(id).getType() == EntityType::ENEMY || entityMan.getEntity(id).getType() == EntityType::SPAWNER || entityMan.getEntity(id).getGameObjectType() == GameObjectType::DRONE_ENEMY) {
        --WorldElementsData::enemiesInWorld;
    }

    entityMan.eraseEntityByID(id);
}



std::unordered_map<int, Entity> &GameEngine::getEntities() {

    return entityMan.getEntities();
}

/*
std::vector<int> &GameEngine::getEntitiesToUpdate() {
    return entityMan.getEntitiesToUpdate();
}

void GameEngine::addEntityToUpdate(const int id) {
    entityMan.addEntityToUpdate(id);
}

void GameEngine::clearEntitiesToUpdate() {
    entityMan.clearEntitiesToUpdate();
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
