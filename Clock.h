//
// Created by arnito on 30/10/16.
//

#ifndef GENLAND_CLOCK_H
#define GENLAND_CLOCK_H


class Clock {
public:
    Clock();
    ~Clock();
    void Update(float delta);
    int day;
    int hour;
    float min;
};


#endif //GENLAND_CLOCK_H
