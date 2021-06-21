#pragma once

#include <SFML/Audio.hpp>
#include <fac/SoundFacade.hpp>
#include <unordered_map>
#include <string>

class GameEngine;

class SoundSFMLEngine : public SoundFacade {
public:
	SoundSFMLEngine();
	~SoundSFMLEngine();

	void playSound(Sound& soundPath) override;
	void playMusic(Sound& musicPath) override;

	void stopAllMusic() override;
	void pauseAllMusic() override;
	void playAllMusic() override;
	//void updateMusic(Sound musica, nuevo estado (pausa, play, stop)) override;

	void loadSound(std::string soundPath) override;
	void loadSoundBuffer(std::string soundBufferPath) override;
	void loadMusic(std::string musicPath) override;

	bool existsSound(std::string soundPath) const override;
	bool existsSoundBuffer(std::string soundBufferPath) const override;
	bool existsMusic(std::string musicPath) const override;


private:
	std::unordered_map<std::string, sf::Sound> soundMap;
	std::unordered_map<std::string, sf::Music> musicMap; // Stores the data of the sounds
	std::unordered_map<std::string, sf::SoundBuffer> soundBufferMap; // Stores the data of the sounds
};