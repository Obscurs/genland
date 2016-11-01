//
// Created by arnito on 30/10/16.
//

#include "Clock.h"
#include <iostream>
Clock::Clock(){
    day=0;
    hour=0;
    min=15;
}


Clock::~Clock() {
}
void Clock::Update(float delta){
    min+=delta;
    if(min>60) {
        hour+= (int)(min/60);
        min=min-60;
    }
    if(hour>24){
        day += hour/24;
        hour = hour-24;
    }
    //std::cout << min << std::endl;

}