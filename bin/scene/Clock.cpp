//
// Created by arnito on 30/10/16.
//

#include "Clock.h"
#include <iostream>
Clock::Clock(){
    day=0;
    hour=0;
    min=15;
    _season = WINTER;
    _dayTime = MORNING;
    _dayTimeFactor =0;
    _lightFactor = 0;
    UpdateDayTimeIntervals();
    _seasonTimeIntervals[0] =70;
    _seasonTimeIntervals[1] =160;
    _seasonTimeIntervals[2] =250;
    _seasonTimeIntervals[3] =340;
    _seasonTimeIntervals[4] =360;
}


Clock::~Clock() {
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
    min+=delta*20;
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

    if(hour< _dayTimeIntervals[0]) _dayTime = MORNING;
    else if(hour <_dayTimeIntervals[1]) _dayTime = AFTERNOON;
    else if(hour < _dayTimeIntervals[2]) _dayTime = EVENING;
    else if(hour < _dayTimeIntervals[3]) _dayTime = NIGHT;
    else _dayTime = MORNING;

    _seasonFactor = getFactorOfInterval(_seasonTimeIntervals, day);
    _dayTimeFactor = getFactorOfInterval(_dayTimeIntervals, hour+min/60);
}