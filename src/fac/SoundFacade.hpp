#pragma once



#include <string>
#include <unordered_map>


class GameEngine;

class SoundFacade {
public:

    virtual ~SoundFacade() = default;

    virtual void playSound(std::string) = 0;
    virtual void playMusic(std::string) = 0;


    virtual void loadSound(std::string) = 0;
    virtual void loadSoundBuffer(std::string) = 0;
    virtual void loadMusic(std::string) = 0;

    virtual bool existsSound(std::string) const = 0;
    virtual bool existsSoundBuffer(std::string) const = 0;
    virtual bool existsMusic(std::string) const = 0;
};