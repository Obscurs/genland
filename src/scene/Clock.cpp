//
// Created by arnito on 30/10/16.
//

#include "Clock.h"
#include "../Inputs.h"
#include "Scene.h"
#include "NoiseGenerator.h"
#include "../Resources.h"
#include <iostream>
Clock::Clock(){
    day=0;
    hour=0;
    min=0;
    _clockSpeed=1;
    _season = WINTER;
    _dayTime = MORNING;
    _dayTimeFactor =0;
    _lightFactor = 0;
    _rainFactor = 0;
    UpdateDayTimeIntervals();
    _seasonTimeIntervals[0] =79;
    _seasonTimeIntervals[1] =170;
    _seasonTimeIntervals[2] =261;
    _seasonTimeIntervals[3] =352;
    _seasonTimeIntervals[4] =360;
    _globalHumidity = 0;
    _globalTemperature = 0;

}


Clock::~Clock() {
}
void Clock::setColorToShader(sf::Shader &shader){
    switch(_dayTime){
        case Clock::MORNING:
            shader.setParameter("color", sf::Color::Black);
            shader.setParameter("color2", sf::Color(244, 173, 66));
            break;
        case Clock::AFTERNOON:
            shader.setParameter("color", sf::Color(244, 173, 66));
            shader.setParameter("color2", sf::Color::Yellow);
            break;
        case Clock::EVENING:
            shader.setParameter("color", sf::Color::Yellow);
            shader.setParameter("color2", sf::Color::Blue);
            break;
        case Clock::NIGHT:
            shader.setParameter("color", sf::Color::Blue);
            shader.setParameter("color2", sf::Color::Black);
            break;
    }
    shader.setParameter("factor2", _dayTimeFactor);
    shader.setParameter("factor", _lightFactor);
}
float Clock::getFactorOfInterval(int (&interval)[5], float value){
    int min = 0;
    int max = 0;
    float real_value = 0;
    float amount = 0;
    for(int i = 0; i < 4; i++){
        if(i==3){

            min = interval[i];
            max = interval[i+1]+interval[0];

            if(value>=min) real_value = value-min;
            else real_value = value+interval[i+1]-min;
            //std::cout << max-min << " " << value << " " << real_value << std::endl;
            break;
        }
        else if(interval[i] <= value && interval[i+1] > value){
            min = interval[i];
            max = interval[i+1];
            real_value = value-min;
            break;
        }
    }
    amount = max-min;
    return real_value/amount;
}
void Clock::draw(sf::RenderTarget &target){
    sf::View currentView = target.getView();
    sf::Vector2f centerView = currentView.getCenter();
    sf::Vector2f sizeView = currentView.getSize();

    int widthBar = 256;
    int heightBar = 256;
    float xBar = centerView.x+sizeView.x/2-widthBar;
    float yBar = centerView.y-sizeView.y/2;

    sf::Vector2f pos_player = Scene::getScene()->getPlayer()->getPositionCol();
    float global_temp = Scene::getScene()->getTemperature(pos_player);
    float local_temp = Scene::getScene()->getTemperatureGlobal(pos_player);
    int total_temp = global_temp + local_temp;
    float global_hum = Scene::getScene()->getHumidity(pos_player);
    float local_hum = Scene::getScene()->getHumidityGlobal(pos_player);
    int total_hum = global_hum + local_hum;

    sf::Sprite backgound(*Resources::getTexture("clock_background"));
    backgound.setPosition(xBar,yBar);
    sf::Sprite clock(*Resources::getTexture("clock_clock"));
    clock.setPosition(xBar,yBar);
    sf::Sprite aro(*Resources::getTexture("clock_circle"));
    aro.setPosition(xBar,yBar);
    sf::Sprite n_hour(*Resources::getTexture("clock_neddle_hour"));
    n_hour.setPosition(xBar+136,yBar+96);
    n_hour.setOrigin(sf::Vector2f(136,96));



    float hmod = (((hour) %12)+(min/60.f))/12;
    n_hour.rotate(hmod*360+180);
    sf::Sprite n_temp(*Resources::getTexture("clock_neddle_temp"));
    n_temp.setPosition(xBar,yBar-std::max((std::min(float((total_temp/50.f)*128),120.f)),-100.f));
    sf::Sprite n_season(*Resources::getTexture("clock_neddle_season"));
    n_season.setPosition(xBar+136,yBar+96);
    n_season.setOrigin(sf::Vector2f(136,96));
    float smod = (((day) %365)+(hour/24.f))/365;
    n_season.rotate(smod*360);
    target.draw(backgound);
    target.draw(n_season);
    target.draw(clock);
    target.draw(n_hour);
    target.draw(aro);
    target.draw(n_temp);
    sf::Text text;
    text.setCharacterSize(24);
    text.setColor(sf::Color::White);
    text.setFont(*Resources::getFont("font"));
    //_text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    sf::String str(std::to_string(total_hum)+"%");
    text.setString(str);
    text.setPosition(xBar+45,yBar-5);
    target.draw(text);

    sf::String str2(std::to_string(total_temp));
    text.setString(str2);
    text.setPosition(xBar+192,yBar-5);
    target.draw(text);



    sf::String str3(std::to_string(day));
    text.setCharacterSize(32);
    text.setString(str3);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(xBar+136,yBar+96));
    target.draw(text);
}
void Clock::UpdateDayTimeIntervals(){
    switch(_season){
        case WINTER:
            _dayTimeIntervals[0] = 7;
            _dayTimeIntervals[1] = 13;
            _dayTimeIntervals[2] = 17;
            _dayTimeIntervals[3] = 19;
            _dayTimeIntervals[4] = 22;
            break;
        case SPRING:
            _dayTimeIntervals[0] = 5;
            _dayTimeIntervals[1] = 13;
            _dayTimeIntervals[2] = 18;
            _dayTimeIntervals[3] = 21;
            _dayTimeIntervals[4] = 24;
            break;
        case SUMMER:
            _dayTimeIntervals[0] = 4;
            _dayTimeIntervals[1] = 13;
            _dayTimeIntervals[2] = 20;
            _dayTimeIntervals[3] = 22;
            _dayTimeIntervals[4] = 24;
            break;
        case AUTUMN:
            _dayTimeIntervals[0] = 6;
            _dayTimeIntervals[1] = 12;
            _dayTimeIntervals[2] = 18;
            _dayTimeIntervals[3] = 19;
            _dayTimeIntervals[4] = 23;
            break;
        default:
            _dayTimeIntervals[0] = 5;
            _dayTimeIntervals[1] = 12;
            _dayTimeIntervals[2] = 17;
            _dayTimeIntervals[3] = 21;
            _dayTimeIntervals[4] = 24;
            break;
    }
}


void Clock::update(float delta){
    if (Inputs::KeyBreak(Inputs::Key::ADD)){
        _clockSpeed +=1;
    } else if(Inputs::KeyBreak(Inputs::Key::SUB)){
        _clockSpeed -=1;
    }
    min+=delta*_clockSpeed;
    if(min>60) {
        hour+= (int)(min/60);
        min=min-60;
    }
    if(hour>23){
        day += hour/24;
        hour = hour-24;
    }
    if(day>364){
        day = 0;
    }
    int old_season = _season;
    if(day< _seasonTimeIntervals[0]) _season = WINTER;
    else if(day <_seasonTimeIntervals[1]) _season = SPRING;
    else if(day < _seasonTimeIntervals[2]) _season = SUMMER;
    else if(day < _seasonTimeIntervals[3]) _season = AUTUMN;
    else _season = WINTER;
    if(_season != old_season) UpdateDayTimeIntervals();

    if(hour< _dayTimeIntervals[0]) _dayTime = NIGHT;
    else if(hour <_dayTimeIntervals[1]) _dayTime = MORNING;
    else if(hour < _dayTimeIntervals[2]) _dayTime = AFTERNOON;
    else if(hour < _dayTimeIntervals[3]) _dayTime = EVENING;
    else _dayTime = NIGHT;

    _seasonFactor = getFactorOfInterval(_seasonTimeIntervals, day);
    _dayTimeFactor = getFactorOfInterval(_dayTimeIntervals, hour+min/60);
    switch(_dayTime){
        case MORNING:
            _lightFactor = 0.9-0.5*_dayTimeFactor;
            break;
        case AFTERNOON:
            _lightFactor = 0.4;
            break;
        case EVENING:
            _lightFactor = 0.4;
            break;
        case NIGHT:
            if(_dayTimeFactor>0.5){
                _lightFactor = 0.9;
            }
            else _lightFactor = 0.4f+0.5f*_dayTimeFactor*2.f;
            break;
    }
    float seasonFactorAux = 1-std::abs((_seasonFactor-float(0.5))*2);
    switch(_season){
        case WINTER:
            _globalTemperature = -5*seasonFactorAux;
            _globalHumidity = 0*seasonFactorAux;
            break;
        case SPRING:
            _globalTemperature = 0*seasonFactorAux;
            _globalHumidity = 15*seasonFactorAux;
            break;
        case SUMMER:
            _globalTemperature = 10*seasonFactorAux;
            _globalHumidity = 0*seasonFactorAux;
            break;
        case AUTUMN:
            _globalTemperature = 0*seasonFactorAux;
            _globalHumidity = 0*seasonFactorAux;
            break;
        default:
            _globalTemperature = 0;
            _globalHumidity = 0;
            break;
    }
    _globalTemperature += ((1-_lightFactor*10))-5;

    float valRain = (NoiseGenerator::getNoise("rainSimplex")->valSimplex2D(0, min+60*hour+24*60*day)+_globalHumidity/100);
    _rainFactor = std::max((valRain-0.7f)*3.3f,0.0f);
}