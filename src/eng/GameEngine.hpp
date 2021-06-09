#pragma once

#include "man/EntityManager.hpp"

#include <sys/SystemsIncluder>

#include <ent/Entity.hpp>

#include <com/ComponentsIncluder>

#include <fac/WindowFacade.hpp>
#include <fac/SoundFacade.hpp>

#include <eng/sfml/SFMLEngine.hpp>
#include <eng/sound/SoundSFMLEngine.hpp>

#include <enum/EnumsIncluder>

#include <memory>
#include <chrono>
#include <stack>

#include <sys/System.hpp>

class GameEngine {
public:
    GameEngine();
    ~GameEngine();

    void reset();
    void init();
    void run() ;
    WindowFacade &getWindowFacadeRef();
    SoundFacade &getSoundFacadeRef();
    void setPlaying(const bool p);
    const float getDeltaTime() const;
    void calculateDeltaTime(std::chrono::time_point<std::chrono::system_clock> &then);


    Entity                               &getEntity(int id);
    std::unordered_map<int, Entity> &getEntities();
    void eraseEntityByID                 (int id);

    /*std::vector<int> &getEntitiesToUpdate();
    void addEntityToUpdate(const int id);
    void clearEntitiesToUpdate();*/

    GameState getGameState() const;
    void popGameState();
    void pushGameState(const GameState gs);
    void clearGameStateStack(); // cuidado con esto, no dejar vacio

    EntityManager   entityMan;

private:
    void update();
    void updateWithTimers();
    void updateSound();
    void render();

    void setMenuSystems(GameObjectType const menu);
    void setPlayingSystems();

    void updateEntitiesInWindow();

    void manageFPS();

    std::vector<std::unique_ptr<System>> systems;
    std::vector<std::unique_ptr<System>> systemsLate;

    SFMLEngine windowFacade;
    SoundSFMLEngine soundFacade;

    const uint8_t MAX_SYSTEMS       { 255 };

    float deltaTime {0};
    float deltaFromLastUpdate {0};

    uint8_t framesPassed{ 0 };
    float frameTimeCounter{ 0.f };

    // To check process time of the system
    //This works every 5 updates it gets the average value of the time for each system
    std::vector<float *> systemTimes;
    uint32_t timesUpdated_sys = 0;
    std::vector<float> updateTimes;
    std::vector<float> soundTimes;
    std::vector<float> renderTimes;
    std::vector<float> allTimes;
    uint32_t timesUpdated_upd = 0;
    const uint8_t MAX_TIMES = 25;

    bool        playing     {true};

    /*GameState   gameState   {GameState::PLAYING};
    GameState   lastState   {GameState::NOSTATE};*/
    std::stack<GameState> gameStateStack;
    bool gameStateChanged{ false };


    float DELTA_TO_UPDATE = 1.f / 60.f;
};
