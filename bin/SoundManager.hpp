#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include <map>
#include <iostream>
#include <SFML/Audio.hpp>

class SoundManager {

public:
    static void load();
    static void clean();
    static void playSound(std::string name);
    static void playSoundNoRestart(std::string name);
    static void playMusic(std::string name);
    static void playMusicNoRestart(std::string name);
    static void stopMusic(std::string name);
    static void pauseMusic(std::string name);
    static void setLoop(bool loop, std::string name);
    static void setPitch(float pitch, std::string name);
    static void setVolume(float volume, std::string name);
    static void setVolumeSound(float volume, std::string name);
    static void setPosition(float x, float y, float z, std::string name);
    static void setGlobalSoundVolumen(float volume);
    static void setGlobalMusicVolumen(float volume);

private:
    static void addMusic(std::string KEY, std::string PATH);
    static void addSound(std::string key, std::string PATH);

    static std::string currentMusic;
    static std::string currentSound;

    static std::vector<sf::SoundBuffer*> soundBuffers;

    static std::map<std::string, sf::Sound> soundMap;
    static std::map<std::string, sf::Music> musicMap;
    static std::map<std::string, sf::Sound>::iterator sit;
    static std::map<std::string, sf::Music>::iterator mit;

};

#endif // SOUNDMANAGER_HPP
