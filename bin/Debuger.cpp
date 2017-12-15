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

void explodeString(std::vector<std::string>& result, std::string s, char separator){
    std::stringstream ss(s);
    std::string word;
    if (!s.empty())
    {
        while(std::getline(ss,word,separator)){
            result.push_back(word);
        }
    } else {
        std::cout << "estas llegint una linia buida" << std::endl;
    }
}
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
    if(s.size()>0){
        std::vector<std::string> args;
        explodeString(args,s,' ');
        if(args.size()==1){
            if(args[0]== "show_chunk_lines"){
                Debuger::metric1 = "linesChunks";
            } else if(args[0]== "show_tension"){
                Debuger::metric2 = "tension";
            } else if(args[0]== "show_reach_floor"){
                Debuger::metric2 = "reachFloor";
            } else if(args[0]== "show_humidity"){
                Debuger::metric2 = "humidity";
            }else if(args[0]== "show_temperature"){
                Debuger::metric2 = "temperature";
            }
            else if(args[0]== "show_id_tiles"){
                Debuger::metric3 = "id";
            }
            else if(args[0]== "disable_all_metrics"){
                Debuger::metric1="none";
                Debuger::metric2="none";
                Debuger::metric3="none";
            }
        } else {
            if(args[0]== "set_day"){
                Scene *scene = Scene::getScene();
                Clock *c = scene->getClock();
                Date *d = new Date();
                d->day = c->day;
                d->min = c->min;
                d->hour = c->hour;
                c->day = stoi(args[1]);
                scene->getEcosystem(0)->updateWithElapsedTime(d,true);
                scene->getEcosystem(1)->updateWithElapsedTime(d,true);
                scene->getMap()->_chunk_mat[0]->_is_dirty = true;
                scene->getMap()->_chunk_mat[1]->_is_dirty = true;
                scene->getMap()->_chunk_mat[2]->_is_dirty = true;
            } else if(args[0]== "set_hour"){
                Scene *scene = Scene::getScene();
                Clock *c = scene->getClock();
                c->hour = stoi(args[1]);
            } else if(args[0]== "give_item" && args.size()==3){
                std::string item = args[1];
                int amount = stoi(args[2]);
                Player *p = Scene::getScene()->getPlayer();
                p->giveItem(item,amount);
            }
        }
    }


}
void Debuger::Update(const sf::Time& deltatime){
    if(_is_init){
        if(Inputs::KeyBreak(Inputs::TAB)) {
            activated = !activated;
            _terminal.disable();
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
    buffer << "Chunk: " << Map::getChunkIndex(int(pos.x)) << " X: " << int(pos.x) << " Y: " << int(pos.y);

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
    buffer << "Day: " << clk->day << " Hour: " << clk->hour <<" Min: " << int(clk->min) <<  " DayTime: " << clk->_dayTime << " "<< clk->_dayTimeFactor<< " Season: " << clk->_season<< " "<< clk->_seasonFactor << " Rain factor: " << clk->_rainFactor;
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

    buffer << "Ecosystem " << eco << " (" << e1->getInterval().x << " - " << e1->getInterval().y <<") Num Mobs: " << e1->getMobPopulationAndTreshold().x <<  "/" << e1->getMobPopulationAndTreshold().y <<  "/" << e1->getMobPopulationAndTreshold().z << "Num Trees: " << e1->getTreePopulationAndTreshold().x <<  "/" << e1->getTreePopulationAndTreshold().y <<  "/" << e1->getTreePopulationAndTreshold().z;
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
    s->getMobsOnArea(mobs,sf::Vector2i(position),100,eco);
    for(int i =0; i<mobs.size();i++){
        mobs[i]->_focusDebug = true;
    }
    if(mobs.size()>0) {
        std::stringstream buffer2;
        buffer2 << "Mob Position colision: " << mobs[0]->_positionCol.x << " " << mobs[0]->_positionCol.y << " Size colision: " << mobs[0]->_sizeCol.x << " " << mobs[0]->_sizeCol.y << " Radius: " << mobs[0]->getGenetics()->_distanceMaxReproduce << "/" << mobs[0]->getGenetics()->_distanceMaxMove;
        std::string string(buffer2.str());
        sf::String str(string);
        _text.setString(str);
        _text.setPosition(centerView.x-sizeView.x/2, centerView.y-sizeView.y/2+_displace);
        _window->draw(_text);
        _displace = _displace + DISPLACEMENT;

        std::stringstream buffer3;
        buffer3 << "Race: " << mobs[0]->getGenetics()->_race << " Life: " << mobs[0]->_life << "/" << mobs[0]->getGenetics()->_health << " Hunger: " << mobs[0]->_hunger << "/" << mobs[0]->getGenetics()->_foodNeeds << " Age: " << mobs[0]->_age << "/" << mobs[0]->getGenetics()->_age << " Target: " << (mobs[0]->_target !=nullptr);
        std::string string3(buffer3.str());
        sf::String str3(string3);
        _text.setString(str3);
        _text.setPosition(centerView.x-sizeView.x/2, centerView.y-sizeView.y/2+_displace);
        _window->draw(_text);
        _displace = _displace + DISPLACEMENT;

        std::stringstream buffer4;
        buffer4 << "Food: ";
        for(int i =0; i< mobs[0]->getGenetics()->_food.size();i++){
            buffer4 << mobs[0]->getGenetics()->_food[i] << " ";
        }
        buffer4 << "\n Enemys: ";
        for(int i =0; i< mobs[0]->getGenetics()->_enemys.size();i++){
            buffer4 << mobs[0]->getGenetics()->_enemys[i] << " ";
        }
        buffer4 << "\n Friends: ";
        for(int i =0; i< mobs[0]->getGenetics()->_friends.size();i++){
            buffer4 << mobs[0]->getGenetics()->_friends[i] << " ";
        }
        buffer4 << "\n Neutral: ";
        for(int i =0; i< mobs[0]->getGenetics()->_neutral.size();i++){
            buffer4 << mobs[0]->getGenetics()->_neutral[i] << " ";
        }
        std::string string4(buffer4.str());
        sf::String str4(string4);
        _text.setString(str4);
        _text.setPosition(centerView.x-sizeView.x/2, centerView.y-sizeView.y/2+_displace);
        _window->draw(_text);
        _displace = _displace + DISPLACEMENT*4;
    } else {

        std::vector<Tree*> trees;
        s->getTreesOnArea(trees,sf::Vector2i(position),100,eco);
        if(trees.size() >0){
            int pos_l = 0;
            int pos_r = 0;
            if(trees[0]->_left_n != nullptr) pos_l = trees[0]->_left_n->_position.x;
            if(trees[0]->_right_n != nullptr) pos_r = trees[0]->_right_n->_position.x;
            std::stringstream buffer2;
            buffer2 << "Tree Position: " << trees[0]->_position.x << " " << trees[0]->_position.y << " padding: " << trees[0]->_min_x << ", " << trees[0]->_max_x <<" Health: " << trees[0]->_life <<" Reproduce countdown: " << trees[0]->_timeToReproduce << "\n";
            buffer2 << "Damage time: " << trees[0]->_debug_last_damage_time << " Damage distance: " << trees[0]->_debug_last_damage_distance << " Damage temp: " << trees[0]->_debug_last_damage_temp << " Damage humid " << trees[0]->_debug_last_damage_hum << "\n";
            buffer2 << "Tree Fathers: " << pos_l << " " << pos_r << "\n";
            TreeGenetics *gens = trees[0]->getGenetics();
            buffer2 << "Tree corb: " << gens->_corb << " Tree amplitude: " << gens->_amplitude << " Tree height: " << gens->_height << "\n";
            buffer2 << "Branch amount: " << gens->_branchAmount << " Curve branch: " << gens->_curveBranch << " Branch size: " << gens->_sizeBranch << "\n";
            buffer2 << "Type leave: " << gens->_typeLeave << " Amount leave: " << gens->_amountLeave << " Density leave: " << gens->_densityLeave << "\n";
            buffer2 << "Life: " << gens->_health << " Cold res " << gens->_cold << " Hot res " << gens->_hot << " Humidity res " << gens->_humidity << " Gen strenght " << gens->_strenghtGen << " Reproduce freq " << gens->_reproduceFactor;
            std::string string(buffer2.str());
            sf::String str(string);
            _text.setString(str);
            _text.setPosition(centerView.x-sizeView.x/2, centerView.y-sizeView.y/2+_displace);
            _window->draw(_text);
            _displace = _displace + DISPLACEMENT*7;
        }


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
