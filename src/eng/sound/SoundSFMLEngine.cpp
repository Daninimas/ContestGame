#include "SoundSFMLEngine.hpp"
#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>
#include <iostream>

SoundSFMLEngine::SoundSFMLEngine() {

}

SoundSFMLEngine::~SoundSFMLEngine() {

}

void SoundSFMLEngine::playSound(Sound& sound) {
	if (existsSound(sound.soundPath)) {
		sf::Sound& SFMLsound = soundMap[sound.soundPath];
		// Set sound data
		SFMLsound.setLoop(sound.repeat);
		SFMLsound.setPitch(sound.pitch);
		SFMLsound.setVolume(sound.volume);
		
		SFMLsound.play();
	}
}

void SoundSFMLEngine::playMusic(Sound& music) {
	if (existsMusic(music.soundPath)) {
		sf::Music& SFMLmusic = musicMap[music.soundPath];
		// Set sound data
		SFMLmusic.setLoop(music.repeat);
		SFMLmusic.setPitch(music.pitch);
		SFMLmusic.setVolume(music.volume);

		SFMLmusic.play();
	}
}

void SoundSFMLEngine::stopAllMusic() {
	for (auto& music : musicMap) {
		music.second.stop();
	}
}

void SoundSFMLEngine::pauseAllMusic() {
	for (auto& music : musicMap) {
		music.second.pause();
	}
}

void SoundSFMLEngine::playAllMusic() {
	for (auto& music : musicMap) {
		music.second.play();
	}
}




void SoundSFMLEngine::loadSound(std::string soundPath) {

	if (soundPath != "" && !existsSound(soundPath)) {
		if (!existsSoundBuffer(soundPath)) {
			loadSoundBuffer(soundPath);
		}

		soundMap.emplace( std::piecewise_construct, std::forward_as_tuple(soundPath), std::forward_as_tuple(soundBufferMap[soundPath]) );  // Creates the sound and sets the buffer
	}
}

void SoundSFMLEngine::loadSoundBuffer(std::string soundBufferPath) {
	if (soundBufferPath != "" && !existsSoundBuffer(soundBufferPath)) {
		soundBufferMap.emplace( std::piecewise_construct, std::forward_as_tuple(soundBufferPath), std::forward_as_tuple() );  // Creates the buffer object

		// Load the data to the buffer
		if (!soundBufferMap[soundBufferPath].loadFromFile(soundBufferPath)) {
			std::cout << "Error loading the sound buffer: " << soundBufferPath << "\n";
			soundBufferMap.erase(soundBufferPath);
		}
	}
}

void SoundSFMLEngine::loadMusic(std::string musicPath) {
	if (musicPath != "" && !existsMusic(musicPath)) {
		musicMap.emplace(std::piecewise_construct, std::forward_as_tuple(musicPath), std::forward_as_tuple());  // Creates the buffer object

		if (!musicMap[musicPath].openFromFile(musicPath)) {
			std::cout << "Error loading the music: " << musicPath << "\n";
			musicMap.erase(musicPath);
		}

	}
}




bool SoundSFMLEngine::existsSound(std::string soundPath) const {
	if (soundMap.find(soundPath) == soundMap.end())
		return false;
	return true;
}

bool SoundSFMLEngine::existsSoundBuffer(std::string soundBufferPath) const {
	if (soundBufferMap.find(soundBufferPath) == soundBufferMap.end())
		return false;
	return true;
}

bool SoundSFMLEngine::existsMusic(std::string musicPath) const {
	if (musicMap.find(musicPath) == musicMap.end())
		return false;
	return true;
}
