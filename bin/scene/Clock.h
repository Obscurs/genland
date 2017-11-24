//
// Created by arnito on 30/10/16.
//

#ifndef GENLAND_CLOCK_H
#define GENLAND_CLOCK_H
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "world/Simplex2d.h"

class Clock {
public:
    Clock();
    ~Clock();
    void update(float delta);
    void draw(sf::RenderTarget &target);
    void setColorToShader(sf::Shader &shader);
    int day;
    int hour;
    float min;
    enum DayTimes {
        MORNING = 0,
        AFTERNOON,
        EVENING,
        NIGHT,
    };
    enum Seasons {
        WINTER = 0,
        SPRING,
        SUMMER,
        AUTUMN
    };
    void UpdateDayTimeIntervals();
    int _dayTimeIntervals[5];
    int _seasonTimeIntervals[5];
    int _season;
    int _dayTime;
    int _clockSpeed;
    float _dayTimeFactor;
    float _seasonFactor;
    float _lightFactor;

    float _globalTemperature;
    float _globalHumidity;

    float _rainFactor;
private:
    float getFactorOfInterval(int (&interval)[5], float value);
};


#endif //GENLAND_CLOCK_H
