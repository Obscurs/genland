#include "SoundManager.hpp"

const std::string STOP_CURRENT_MUSIC = "stop_current_music";
const std::string CONTINUE_CURRENT_MUSIC = "continue_current_music";

std::string SoundManager::currentMusic;
std::string SoundManager::currentSound;

std::vector<sf::SoundBuffer*> SoundManager::soundBuffers;

std::map<std::string, sf::Sound> SoundManager::soundMap;
std::map<std::string, sf::Music> SoundManager::musicMap;
std::map<std::string, sf::Sound>::iterator SoundManager::sit;
std::map<std::string, sf::Music>::iterator SoundManager::mit;


void SoundManager::addMusic(std::string KEY, std::string PATH){
    if(! musicMap[KEY].openFromFile(PATH) ) std::cout << "Music not loaded for key " << KEY << " -> path: " << PATH << std::endl;
}

void SoundManager::addSound(std::string key, std::string PATH){
    sf::SoundBuffer *sb = new sf::SoundBuffer();
    soundBuffers.push_back(sb);
    int last = soundBuffers.size()-1;
    if(! sb->loadFromFile(PATH)){ std::cout << "Fail loading " << key << " with " << PATH << std::endl;};
    soundMap[key].setBuffer(*soundBuffers[last]);
}
void SoundManager::clean(){
    for(int i=0; i<soundBuffers.size(); ++i){
        delete(soundBuffers[i]);
    }
}
void SoundManager::load(){

    //MUSUIC
    addMusic("ambient1", "resources/sounds/ambient.wav");
    std::cout << "music loaded" << std::endl;

    //SOUNDS


    addSound("click", "resources/sounds/click2.wav");
    addSound("punch", "resources/sounds/punch.ogg");
    addSound("dirt", "resources/sounds/dirt.wav");
    addSound("leaf", "resources/sounds/leaf.wav");
    addSound("sword", "resources/sounds/sword2.wav");
    addSound("pick", "resources/sounds/pick3.wav");
    addSound("stones", "resources/sounds/stones4.wav");
    addSound("step", "resources/sounds/step.wav");
    addSound("craft", "resources/sounds/craft.wav");
    addSound("place", "resources/sounds/place.wav");
    addSound("bubble", "resources/sounds/bubble.wav");
    addSound("placeBloc", "resources/sounds/placeBloc.wav");
    addSound("p_hurt_1", "resources/sounds/phurt1.wav");
    addSound("p_hurt_2", "resources/sounds/phurt2.wav");
    addSound("p_hurt_3", "resources/sounds/phurt3.wav");
    addSound("m_attack_1", "resources/sounds/bite1.wav");
    addSound("m_attack_2", "resources/sounds/bite2.wav");
    addSound("m_attack_3", "resources/sounds/bite3.wav");
    addSound("m_attack_4", "resources/sounds/bite4.wav");
    addSound("rain0", "resources/sounds/rain_start.wav");
    addSound("rain1", "resources/sounds/rain_low.wav");
    addSound("rain2", "resources/sounds/rain_medium.wav");
    addSound("rain3", "resources/sounds/rain_intense.wav");
    addSound("day", "resources/sounds/day.wav");
    addSound("night", "resources/sounds/night.wav");
    addSound("wind", "resources/sounds/wind.wav");
    addSound("cave", "resources/sounds/cave.wav");
    addSound("fire", "resources/sounds/fire.wav");
    currentSound = "";
    currentMusic = "";
    std::cout << "sound loaded" << std::endl;
}



void SoundManager::playSound(std::string name){
    sit = soundMap.find(name);
    if (sit != soundMap.end()) {
        (sit->second).play();
        currentSound = name;
    }
}
void SoundManager::playSoundNoRestart(std::string name){

    sit = soundMap.find(name);
    if (sit != soundMap.end()) {
        if((sit->second).getStatus() == sf::Sound::Stopped) currentSound = "";
    }

    if(currentSound == ""){
        sit = soundMap.find(name);
        if (sit != soundMap.end()) {
            (sit->second).play();
            currentSound = name;
        }
    }

}
void SoundManager::playMusicNoRestart(std::string name){

    mit = musicMap.find(currentMusic);
    if (mit != musicMap.end()) {
        if((mit->second).getStatus() == sf::Sound::Stopped) currentMusic = "";
    }

    if( "" == currentMusic) {
        stopMusic(currentMusic);
            mit = musicMap.find(name);
            if (mit != musicMap.end()) {
                (mit->second).play();
                currentMusic = name;
            } else std::cout << "trying to play wrong music: " << name << std::endl;
    }

}
void SoundManager::playMusic(std::string name){
    if( name != currentMusic) {
        stopMusic(currentMusic);
        if(name != STOP_CURRENT_MUSIC){
            mit = musicMap.find(name);
            if (mit != musicMap.end()) {
                (mit->second).play();
                currentMusic = name;
            } else std::cout << "trying to play wrong music: " << name << std::endl;
        } else std::cout << "STOP_CURRENT_MUSIC music" << std::endl;
    }
}

void SoundManager::stopMusic(std::string name){
    if(name  != CONTINUE_CURRENT_MUSIC && name != STOP_CURRENT_MUSIC) {
        mit = musicMap.find(name);
        if (mit != musicMap.end()) {
            (mit->second).stop();
            currentMusic = STOP_CURRENT_MUSIC;
        }
    }
}

void SoundManager::pauseMusic(std::string name){
    mit = musicMap.find(name);
    if (mit != musicMap.end()) (mit->second).pause();

}

void SoundManager::setLoop(bool loop, std::string name){
    mit = musicMap.find(name);
    if (mit != musicMap.end()) (mit->second).setLoop(loop);

}

void SoundManager::setPitch(float pitch, std::string name){
    mit = musicMap.find(name);
    if (mit != musicMap.end()) (mit->second).setPitch(pitch);
}

void SoundManager::setVolume(float volume, std::string name){
    mit = musicMap.find(name);
    if (mit != musicMap.end()) (mit->second).setVolume(volume);
}
void SoundManager::setVolumeSound(float volume, std::string name){
    sit = soundMap.find(name);
    if (sit != soundMap.end()) (sit->second).setVolume(volume);
}

void SoundManager::setPosition(float x, float y, float z, std::string name){
    mit = musicMap.find(name);
    if (mit != musicMap.end()) (mit->second).setPosition(x,y,z);
}


void SoundManager::setGlobalSoundVolumen(float volume) {
    for (auto it = soundMap.begin(); it != soundMap.end(); ++it) (it->second).setVolume(volume);
}

void SoundManager::setGlobalMusicVolumen(float volume) {
    for (auto it = musicMap.begin(); it != musicMap.end(); ++it) (it->second).setVolume(volume);
}
