//
// Created by arnito on 30/10/16.
//

#ifndef GENLAND_CLOCK_H
#define GENLAND_CLOCK_H
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

class Clock {
public:
    Clock();
    ~Clock();
    void Update(float delta);
    void SetColorToShader(sf::Shader &shader);
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

    float _dayTimeFactor;
    float _seasonFactor;
    float _lightFactor;
private:
    float getFactorOfInterval(int (&interval)[5], float value);
};


#endif //GENLAND_CLOCK_H
