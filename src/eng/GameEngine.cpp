#include "GameEngine.hpp"

#include <random>
#include <time.h>
#include <cstdint>

#include <iostream>


#include <sys/SystemsIncluder>


const bool SHOW_TIMERS      = false;
const bool CHECK_SYSTEMS    = false;
const float DELTA_TO_UPDATE = 1.f / 60.f;

GameEngine::GameEngine()
    : windowFacade(800, 600, false)/*, soundFacade()*/ {


    srand(time(NULL)); // initialize the random seed
    //init();
    systems.reserve(MAX_SYSTEMS);

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
    init();

}

void GameEngine::init() {
    setGameState(GameState::PLAYING);

    StaticEntitiesSystem staticSystem{};
    staticSystem.init(*this);

    //systems.emplace_back(std::make_unique<TimeSystem>());                   //#00
}

void GameEngine::run() {
    // For the interpolation
    std::chrono::time_point<std::chrono::system_clock> then;
    then = std::chrono::system_clock::now();

    while (playing) {

        if (lastState != gameState) {
            // State reciently changed. Systems vector must be updated
            systems.clear();
            switch (gameState) {
            case GameState::GAMEOVER:
                //systems.emplace_back(std::make_unique<GameOverSystem>());
                break;

            case GameState::PLAYING:
                init();
                break;
            }

            lastState = gameState;
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
}

void GameEngine::updateSound() {
    //soundFacade.update(*this);
}

void GameEngine::render() {
    windowFacade.render(*this);
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


    then = now;

    // FPS counter
    //int fps   = 1 / elapsed_time.count();
    //std::cout << "FPS: " << fps << std::endl;
}

WindowFacade &GameEngine::getWindowFacadeRef() {
    return windowFacade;
}
/*
SoundFacade &GameEngine::getSoundFacadeRef() {
    //return soundFacade;
}*/



Entity &GameEngine::getEntity(int id) {
    return entityMan.getEntity(id);
}



void GameEngine::eraseEntityByID(int id) {
    windowFacade.eraseEntity(id);


    // Removing the entity ID on Components
    //getSoundFacadeRef().setParameterEventByID(id, STOP_SOUND);

    entityMan.eraseEntityByID(id);
}



std::unordered_map<int, Entity> &GameEngine::getEntities() {

    return entityMan.getEntities();
}


std::vector<int> &GameEngine::getEntitiesToUpdate() {
    return entityMan.getEntitiesToUpdate();
}

void GameEngine::addEntityToUpdate(const int id) {
    entityMan.addEntityToUpdate(id);
}

void GameEngine::clearEntitiesToUpdate() {
    entityMan.clearEntitiesToUpdate();
}

GameState GameEngine::getGameState() const {
    return gameState;
}

void GameEngine::setGameState(const GameState gs) {
    setLastState(gameState);
    gameState = gs;
}

void GameEngine::setLastState(const GameState gs) {
    lastState = gs;
}
