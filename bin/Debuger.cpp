//
// Created by arnito on 25/04/17.
//

#include "Debuger.h"
#include <iostream>
#include <fstream>
#include <sstream>

sf::Text Debuger::_text;
float Debuger::_fpsTimer;
int Debuger::_fpsCount;
int Debuger::_fpsCount2;
int Debuger::_old_fps = 0;
std::string Debuger::metric1 = "none";
std::string Debuger::metric2 = "none";
std::string Debuger::metric3 = "none";
sf::Clock Debuger::_clock;
sf::RenderWindow *Debuger::_window = NULL;
Scene *Debuger::_scene = NULL;
bool Debuger::_is_init = false;
bool Debuger::activated = false;
int Debuger::_displace = 0;
std::vector<sf::Time> Debuger::_clockMarks;
std::vector<std::string> Debuger::_clockNames;

void Debuger::InsertClockMark(std::string name){
    _clockNames.push_back(name);
    _clockMarks.push_back(_clock.getElapsedTime());
}
void Debuger::Init(sf::RenderWindow &window, Scene &scene){
    _is_init = true;
    _window = &window;
    _scene = &scene;
    _clock.restart();
    activated = false;
    InitFpsText();
}
void Debuger::Update(const sf::Time& deltatime){
    if(_is_init){
        _displace = 0;
        UpdateFpsText(deltatime);
        _clockMarks.clear();
        _clockNames.clear();
        _clock.restart();
    }
}
void Debuger::DrawClockMarks(){
    sf::View currentView    = _window->getView();
    sf::Vector2f centerView = currentView.getCenter();
    sf::Vector2f sizeView   = currentView.getSize();
    _text.setPosition(centerView.x-sizeView.x/2, centerView.y-sizeView.y/2+_displace);

    float time_pre = 0;
    std::vector<float> times(_clockMarks.size());
    for(int i=0; i< _clockMarks.size(); i++){
        float time_curr = _clockMarks[i].asSeconds();
        times[i] = time_curr-time_pre;
        time_pre = time_curr;
    }

    std::stringstream buffer;
    for(int i =0; i< times.size(); i++){
        buffer << _clockNames[i] << ": " << int(times[i]/time_pre*100) <<"% ";
    }
    std::string string(buffer.str());
    sf::String str(string);
    _text.setString(str);
    _window->draw(_text);
    _displace = _displace + DISPLACEMENT;
}

void Debuger::DrawPlayerStats(){
    sf::View currentView    = _window->getView();
    sf::Vector2f centerView = currentView.getCenter();
    sf::Vector2f sizeView   = currentView.getSize();
    _text.setPosition(centerView.x-sizeView.x/2, centerView.y-sizeView.y/2+_displace);
    sf::Vector2f pos = _scene->player.GetPosition();
    std::stringstream buffer;
    buffer << "X: " << int(pos.x) << " Y: " << int(pos.y);

    std::string string(buffer.str());
    sf::String str(string);
    _text.setString(str);
    _window->draw(_text);
    _displace = _displace + DISPLACEMENT;
}

void Debuger::DrawBiomeStats(){
    sf::View currentView    = _window->getView();
    sf::Vector2f centerView = currentView.getCenter();
    sf::Vector2f sizeView   = currentView.getSize();
    _text.setPosition(centerView.x-sizeView.x/2, centerView.y-sizeView.y/2+_displace);
    sf::Vector2f pos = _scene->player.GetPosition();
    std::stringstream buffer;
    int temperature = _scene->getTemperature(pos);
    int humidity = _scene->getHumidity(pos);
    float mountFactor = _scene->getMountFactor(pos);
    std::string bioma = "STANDARD";
    if(mountFactor>0) {
        if(temperature <0) bioma = "ICE MOUNTAIN";
        else bioma = "MOUNTAIN";
    }
    else{
        if(temperature >30){
            if(humidity >70) bioma = "JUNGLE";
            else if(humidity <40) bioma = "DESERT";
        }
        else{
            if(humidity >70) bioma = "FOREST";
            else if(humidity <40) bioma = "PLAINS";
        }
    }
    buffer << "Bioma: " << bioma << " Temperature: " << temperature <<"(" <<temperature+_scene->clock._globalTemperature <<")" << " Humidity: " << humidity <<"(" <<humidity+_scene->clock._globalHumidity <<")"<< " Mountain: " << mountFactor;

    std::string string(buffer.str());
    sf::String str(string);
    _text.setString(str);
    _window->draw(_text);
    _displace = _displace + DISPLACEMENT;
}
void Debuger::DrawWorldStats(){
    sf::View currentView    = _window->getView();
    sf::Vector2f centerView = currentView.getCenter();
    sf::Vector2f sizeView   = currentView.getSize();
    _text.setPosition(centerView.x-sizeView.x/2, centerView.y-sizeView.y/2+_displace);
    std::stringstream buffer;
    buffer << "Day: " << _scene->clock.day << " Hour: " << _scene->clock.hour <<" Min: " << int(_scene->clock.min) <<  " DayTime: " << _scene->clock._dayTime << " "<< _scene->clock._dayTimeFactor<< " Season: " << _scene->clock._season<< " "<< _scene->clock._seasonFactor;
    std::string string(buffer.str());
    sf::String str(string);
    _text.setString(str);
    _window->draw(_text);
    _displace = _displace + DISPLACEMENT;
}
void Debuger::DrawFps(){
    sf::View currentView    = _window->getView();
    sf::Vector2f centerView = currentView.getCenter();
    sf::Vector2f sizeView   = currentView.getSize();
    _text.setPosition(centerView.x-sizeView.x/2, centerView.y-sizeView.y/2+_displace);
    char buffer[40];
    if(_fpsTimer > 1) {
        _old_fps = _fpsCount2 / _fpsCount;
        _fpsTimer = 0;
        _fpsCount = 0;
        _fpsCount2 = 0;

    }
    sprintf(buffer, "%i", _old_fps);
    std::string string(buffer);
    sf::String str(string);
    _text.setString(str);
    _window->draw(_text);
    _displace = _displace +DISPLACEMENT;
}
void Debuger::Draw(){
    if(_is_init){

        DrawFps();
        DrawClockMarks();
        DrawPlayerStats();
        DrawWorldStats();
        DrawBiomeStats();
    }
}
void Debuger::InitFpsText() {
    _text.setCharacterSize(24);
    _text.setColor(sf::Color::Red);
    _text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    sf::String str("no data");
    _text.setString(str);

    _text.setFont(*Resources::getFont("debugFont"));

    _fpsTimer  = 0;
    _fpsCount  = 0;
    _fpsCount2 = 0;
}

void Debuger::UpdateFpsText(const sf::Time& deltatime) {
    float seconds = deltatime.asSeconds();
    float fps = 1.f / seconds;

    _fpsCount += 1;
    _fpsCount2 += fps;
    _fpsTimer += seconds;
}
