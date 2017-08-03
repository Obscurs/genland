//
// Created by arnito on 30/10/16.
//

#include "Clock.h"
#include "../Inputs.h"
#include <iostream>
Clock::Clock(std::string seed){
    day=0;
    hour=7;
    min=15;
    _clockSpeed=1;
    _season = WINTER;
    _dayTime = MORNING;
    _dayTimeFactor =0;
    _lightFactor = 0;
    _rainFactor = 0;
    UpdateDayTimeIntervals();
    _seasonTimeIntervals[0] =70;
    _seasonTimeIntervals[1] =160;
    _seasonTimeIntervals[2] =250;
    _seasonTimeIntervals[3] =340;
    _seasonTimeIntervals[4] =360;
    _globalHumidity = 0;
    _globalTemperature = 0;
    std::mt19937 generator;
    generator.seed(std::stoi(seed));
    _rainSimplex = new Simplex2d(&generator, 150.0f, 0.0f, 1.0f);
}


Clock::~Clock() {
}

void Clock::SetColorToShader(sf::Shader &shader){
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
void Clock::UpdateDayTimeIntervals(){
    switch(_season){
        case WINTER:
            _dayTimeIntervals[0] = 5;
            _dayTimeIntervals[1] = 12;
            _dayTimeIntervals[2] = 17;
            _dayTimeIntervals[3] = 21;
            _dayTimeIntervals[4] = 24;
            break;
        case SPRING:
            _dayTimeIntervals[0] = 5;
            _dayTimeIntervals[1] = 12;
            _dayTimeIntervals[2] = 17;
            _dayTimeIntervals[3] = 21;
            _dayTimeIntervals[4] = 24;
            break;
        case SUMMER:
            _dayTimeIntervals[0] = 5;
            _dayTimeIntervals[1] = 12;
            _dayTimeIntervals[2] = 17;
            _dayTimeIntervals[3] = 21;
            _dayTimeIntervals[4] = 24;
            break;
        case AUTUMN:
            _dayTimeIntervals[0] = 5;
            _dayTimeIntervals[1] = 12;
            _dayTimeIntervals[2] = 17;
            _dayTimeIntervals[3] = 21;
            _dayTimeIntervals[4] = 24;
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
void Clock::Update(float delta){
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
            _globalTemperature = -20*seasonFactorAux;
            _globalHumidity = 0*seasonFactorAux;
            break;
        case SPRING:
            _globalTemperature = 0*seasonFactorAux;
            _globalHumidity = 15*seasonFactorAux;
            break;
        case SUMMER:
            _globalTemperature = 20*seasonFactorAux;
            _globalHumidity = 0*seasonFactorAux;
            break;
        case AUTUMN:
            _globalTemperature = 0*seasonFactorAux;
            _globalHumidity = -5*seasonFactorAux;
            break;
        default:
            _globalTemperature = 0;
            _globalHumidity = 0;
            break;
    }
    _globalTemperature += ((1-_lightFactor*14))-7;

    float valRain = (_rainSimplex->valSimplex2D(0, min+60*hour+24*60*day)+_globalHumidity/100);
    _rainFactor = std::max((valRain-0.7f)*3.3f,0.0f);
}