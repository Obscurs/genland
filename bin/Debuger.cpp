//
// Created by arnito on 25/04/17.
//

#include "Debuger.h"
#include "Inputs.h"
#include "scene/Scene.h"
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
bool Debuger::_is_init = false;
bool Debuger::activated = false;
int Debuger::_displace = 0;
Terminal Debuger::_terminal;
std::vector<sf::Time> Debuger::_clockMarks;
std::vector<std::string> Debuger::_clockNames;

void Debuger::InsertClockMark(std::string name){
    _clockNames.push_back(name);
    _clockMarks.push_back(_clock.getElapsedTime());
}
void Debuger::Init(sf::RenderWindow &window){
    _is_init = true;
    _window = &window;
    _clock.restart();
    activated = false;
    InitFpsText();
}
bool Debuger::isTerminalActive(){
    return _terminal.isActive();
}
void Debuger::setTerminalText(std::string s) {
    _terminal.setText(s);
}
std::string Debuger::getTerminalText(){
    return _terminal.getText();
}
void Debuger::interpInstruction(std::string s){
    if(s == "show chunk lines"){
        Debuger::metric1 = "linesChunks";
    } else if(s == "show tension"){
        Debuger::metric2 = "tension";
    } else if(s == "show reach floor"){
        Debuger::metric2 = "reachFloor";
    } else if(s == "show humidity"){
        Debuger::metric2 = "humidity";
    }else if(s == "show temperature"){
        Debuger::metric2 = "temperature";
    }
    else if(s == "show id tiles"){
        Debuger::metric3 = "id";
    }
    else if(s == "disable all metrics"){
        Debuger::metric1="none";
        Debuger::metric2="none";
        Debuger::metric3="none";
    }
}
void Debuger::Update(const sf::Time& deltatime){
    if(_is_init){
        if(Inputs::KeyBreak(Inputs::TAB)) {
            activated = !activated;
        }
        if(activated){
            _displace = 0;
            UpdateFpsText(deltatime);
            _clockMarks.clear();
            _clockNames.clear();
            _terminal.Update();
            _clock.restart();
        }

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
    Scene* scene = Scene::getScene();
    sf::View currentView    = _window->getView();
    sf::Vector2f centerView = currentView.getCenter();
    sf::Vector2f sizeView   = currentView.getSize();
    _text.setPosition(centerView.x-sizeView.x/2, centerView.y-sizeView.y/2+_displace);
    sf::Vector2f pos = scene->getPlayer()->GetPosition();
    std::stringstream buffer;
    buffer << "X: " << int(pos.x) << " Y: " << int(pos.y);

    std::string string(buffer.str());
    sf::String str(string);
    _text.setString(str);
    _window->draw(_text);
    _displace = _displace + DISPLACEMENT;
}

void Debuger::DrawBiomeStats(){
    Scene* scene = Scene::getScene();
    sf::View currentView    = _window->getView();
    sf::Vector2f centerView = currentView.getCenter();
    sf::Vector2f sizeView   = currentView.getSize();
    _text.setPosition(centerView.x-sizeView.x/2, centerView.y-sizeView.y/2+_displace);
    sf::Vector2f pos = scene->getPlayer()->GetPosition();
    std::stringstream buffer;


    int temperature = scene->getTemperature(pos);
    int humidity = scene->getHumidity(pos);
    float mountFactor = scene->getMountFactor(pos);

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
    Clock* clk = scene->getClock();
    buffer << "Bioma: " << bioma << " Temperature: " << temperature <<"(" <<temperature+clk->_globalTemperature <<")" << " Humidity: " << humidity <<"(" <<humidity+clk->_globalHumidity <<")"<< " Mountain: " << mountFactor;

    std::string string(buffer.str());
    sf::String str(string);
    _text.setString(str);
    _window->draw(_text);
    _displace = _displace + DISPLACEMENT;
}

void Debuger::DrawWorldStats(){
    Scene* scene = Scene::getScene();
    sf::View currentView    = _window->getView();
    sf::Vector2f centerView = currentView.getCenter();
    sf::Vector2f sizeView   = currentView.getSize();
    _text.setPosition(centerView.x-sizeView.x/2, centerView.y-sizeView.y/2+_displace);
    std::stringstream buffer;
    Clock* clk = scene->getClock();
    buffer << "Day: " << clk->day << " Hour: " << clk->hour <<" Min: " << int(clk->min) <<  " DayTime: " << clk->_dayTime << " "<< clk->_dayTimeFactor<< " Season: " << clk->_season<< " "<< clk->_seasonFactor;
    std::string string(buffer.str());
    sf::String str(string);
    _text.setString(str);
    _window->draw(_text);
    _displace = _displace + DISPLACEMENT;
}
void Debuger::DrawEntitiesStats(int eco){
    sf::View currentView    = _window->getView();
    sf::Vector2f centerView = currentView.getCenter();
    sf::Vector2f sizeView   = currentView.getSize();
    _text.setPosition(centerView.x-sizeView.x/2, centerView.y-sizeView.y/2+_displace);
    std::stringstream buffer;

    Scene* s = Scene::getScene();
    Ecosystem *e1 = s->getEcosystem(eco);

    buffer << "Ecosystem " << eco << " (" << e1->getInterval().x << " - " << e1->getInterval().y <<") Num entities: " << e1->getMobPopulationAndTreshold().x <<  "/" << e1->getMobPopulationAndTreshold().y;
    std::string string(buffer.str());
    sf::String str(string);
    _text.setString(str);
    _window->draw(_text);
    _displace = _displace + DISPLACEMENT;

    sf::Vector2f position = _window->mapPixelToCoords(sf::Mouse::getPosition(*_window));
    float zoom = s->getZoom();
    sf::Vector2f position_center = sf::Vector2f(s->getPlayer()->GetPosition().x+Player::PLAYER_WIDTH/2,s->getPlayer()->GetPosition().y+Player::PLAYER_WIDTH/2);
    sf::Vector2f position_zoomed = (position-position_center)/zoom +position_center;
    position = position_zoomed;
    std::vector<Mob*> mobs;
    std::cout << sf::Vector2i(position).x<< " " << sf::Vector2i(position).y << std::endl;
    s->getMobsOnArea(mobs,sf::Vector2i(position),100,eco);
    if(mobs.size()>0) {
        std::stringstream buffer2;
        buffer2 << "Position colision: " << mobs[0]->_positionCol.x << " " << mobs[0]->_positionCol.y;
        std::string string(buffer2.str());
        sf::String str(string);
        _text.setString(str);
        _text.setPosition(centerView.x-sizeView.x/2, centerView.y-sizeView.y/2+_displace);
        _window->draw(_text);
        _displace = _displace + DISPLACEMENT;
    }
}
void Debuger::sendTerminalInstruction(){
    _terminal.sendTerminalInstruction();
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
    if(_is_init && activated){
        DrawFps();
        DrawClockMarks();
        DrawPlayerStats();
        DrawWorldStats();
        DrawBiomeStats();
        DrawEntitiesStats(0);
        DrawEntitiesStats(1);
        _terminal.draw(*_window,_text);
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
