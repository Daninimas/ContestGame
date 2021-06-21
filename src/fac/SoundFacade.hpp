#pragma once



#include <unordered_map>
#include <tools/Sound.hpp>


class GameEngine;

class SoundFacade {
public:

    virtual ~SoundFacade() = default;

    virtual void playSound(Sound&) = 0;
    virtual void playMusic(Sound&) = 0;

    virtual void stopAllMusic() = 0;
    virtual void pauseAllMusic() = 0;
    virtual void playAllMusic() = 0;

    //virtual void changeMusicState() = 0;

    virtual void loadSound(std::string) = 0;
    virtual void loadSoundBuffer(std::string) = 0;
    virtual void loadMusic(std::string) = 0;

    virtual bool existsSound(std::string) const = 0;
    virtual bool existsSoundBuffer(std::string) const = 0;
    virtual bool existsMusic(std::string) const = 0;
};