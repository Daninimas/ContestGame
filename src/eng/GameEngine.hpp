#pragma once

#include "man/EntityManager.hpp"

#include <sys/SystemsIncluder>

#include <ent/Entity.hpp>

#include <com/ComponentsIncluder>

#include <fac/WindowFacade.hpp>
#include <fac/SoundFacade.hpp>

#include <eng/sfml/SFMLEngine.hpp>

#include <enum/EnumsIncluder>

#include <memory>
#include <chrono>

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
    std::vector<int> &getEntitiesToUpdate();
    void addEntityToUpdate(const int id);
    void clearEntitiesToUpdate();

    GameState getGameState() const;
    void setGameState(const GameState gs);
    void setLastState(const GameState gs);

    EntityManager   entityMan;

private:
    void update();
    void updateWithTimers();
    void updateSound();
    void render();

    std::vector<std::unique_ptr<System>> systems;

    SFMLEngine windowFacade;
    //VibrateEngine soundFacade;

    const uint8_t MAX_SYSTEMS       { 255 };

    float deltaTime {0};
    float deltaFromLastUpdate {0};

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
    GameState   gameState   {PLAYING};
    GameState   lastState   {NOSTATE};

};
