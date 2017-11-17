//
// Created by arnito on 11/10/17.
//

#ifndef GENLAND_MOBGENETICS_H
#define GENLAND_MOBGENETICS_H


#include <vector>

class MobGenetics {
public:
    const static int MOB_RACES = 15;
    MobGenetics();
    MobGenetics(MobGenetics *t1, MobGenetics *t2, float factor);

    //Resistance
    int _cold;      //-> alture
    int _hot;       //-> amplitude
    int _humidity;  //-> density

    //GenFactors
    int _health;
    int _age;
    int _reproduceFactor;
    int _strenghtGen;
    int _distanceMaxMove;
    int _distanceMaxReproduce;
    int _strenght;
    int _jump;
    int _speed;
    int _atackSpeed;
    int _foodNeeds;

    int _size;

    int _race;
    std::vector<int> _friends;
    std::vector<int> _enemys;
    std::vector<int> _food;
    std::vector<int> _neutral;
    void mutate();
private:
    void mixRacePreferences(MobGenetics *t1, MobGenetics *t2, float factor);
    void mutateRaces();
    void valance(int (&amounts)[14]);
    void setRelatedFactors();
    void setFriendYourRace();
};


#endif //GENLAND_MOBGENETICS_H
