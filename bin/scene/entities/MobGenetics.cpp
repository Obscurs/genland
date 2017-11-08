//
// Created by arnito on 11/10/17.
//

#include "MobGenetics.h"
#include <cstdlib>
#include <algorithm>
#include <iostream>
MobGenetics::MobGenetics(){
    int amounts[14] = {rand() % 100};
    valance(amounts);
    _cold = amounts[0];
    _hot = amounts[1];
    _humidity = amounts[2];
    _health = amounts[3];
    _reproduceFactor = amounts[4];
    _strenghtGen = amounts[5];
    _distanceMaxMove = amounts[6];
    _distanceMaxReproduce = amounts[7];
    _strenght = amounts[8];
    _jump = amounts[9];
    _speed = amounts[10];
    _atackSpeed = amounts[11];
    _foodNeeds = amounts[12];
    _age = amounts[13];
    _race = rand() % MOB_RACES;
    for(int i=0; i<MOB_RACES; i++){
        int place = rand() %4;
        if(place ==0){
            _enemys.push_back(i);
        } else if(place ==1){
            _friends.push_back(i);
        } else if(place ==2){
            _food.push_back(i);
        } else if(place ==3){
            _neutral.push_back(i);
        }
    }
    setRelatedFactors();

}
void MobGenetics::mixRacePreferences(MobGenetics *t1, MobGenetics *t2, float factor){
    std::vector<std::pair<int, int> > nonShared1;
    std::vector<std::pair<int, int> > nonShared2;
    if(factor >0.5) _race = t1->_race;
    else _race = t2->_race;
    int auxEnemys[MOB_RACES] = {0};
    int auxFriends[MOB_RACES] = {0};
    int auxFood[MOB_RACES] = {0};
    int auxNeutral[MOB_RACES] = {0};
    for(int i=0; i< t1->_enemys.size(); i++){
        auxEnemys[t1->_enemys[i]] = -2;
    }
    for(int i=0; i< t1->_friends.size(); i++){
        auxFriends[t1->_friends[i]] = -2;
    }
    for(int i=0; i< t1->_food.size(); i++){
        auxFood[t1->_food[i]] = -2;
    }
    for(int i=0; i< t1->_neutral.size(); i++){
        auxNeutral[t1->_neutral[i]] = -2;
    }

    for(int i=0; i< t2->_enemys.size(); i++){
        auxEnemys[t2->_enemys[i]] += 1;
    }
    for(int i=0; i< t2->_friends.size(); i++){
        auxFriends[t2->_friends[i]] += 1;
    }
    for(int i=0; i< t2->_food.size(); i++){
        auxFood[t2->_food[i]] += 1;
    }
    for(int i=0; i< t2->_neutral.size(); i++){
        auxNeutral[t2->_neutral[i]] += 1;
    }
    for(int i=0; i< MOB_RACES; i++){
        if(auxEnemys[i]==-1){
            _enemys.push_back(i);
        } else if(auxEnemys[i]==-2){
            nonShared1.push_back(std::pair<int,int>(i,0));
        } else if(auxEnemys[i]==1){
            nonShared2.push_back(std::pair<int,int>(i,0));
        }
        if(auxFriends[i]==-1){
            _friends.push_back(i);
        } else if(auxFriends[i]==-2){
            nonShared1.push_back(std::pair<int,int>(i,1));
        } else if(auxFriends[i]==1){
            nonShared2.push_back(std::pair<int,int>(i,1));
        }
        if(auxFood[i]==-1){
            _food.push_back(i);
        } else if(auxFood[i]==-2){
            nonShared1.push_back(std::pair<int,int>(i,2));
        } else if(auxFood[i]==1){
            nonShared2.push_back(std::pair<int,int>(i,2));
        }
        if(auxNeutral[i]==-1){
            _neutral.push_back(i);
        } else if(auxNeutral[i]==-2){
            nonShared1.push_back(std::pair<int,int>(i,3));
        } else if(auxNeutral[i]==1){
            nonShared2.push_back(std::pair<int,int>(i,3));
        }
    }
    for(int i=0; i<int(nonShared1.size()*factor); i++){
        if(nonShared1[i].second ==0){
            _enemys.push_back(nonShared1[i].first);
        } else if(nonShared1[i].second ==1){
            _friends.push_back(nonShared1[i].first);
        } else if(nonShared1[i].second ==2){
            _food.push_back(nonShared1[i].first);
        } else if(nonShared1[i].second ==3){
            _neutral.push_back(nonShared1[i].first);
        }
        for(int j =0; j <nonShared2.size(); j++){
            if(nonShared1[i].first == nonShared2[j].first) {
                nonShared2.erase(nonShared2.begin()+j);
                break;
            }
        }
    }
    for(int i=0; i<nonShared2.size(); i++){
        if(nonShared2[i].second ==0){
            _enemys.push_back(nonShared2[i].first);
        } else if(nonShared2[i].second ==1){
            _friends.push_back(nonShared2[i].first);
        } else if(nonShared2[i].second ==2){
            _food.push_back(nonShared2[i].first);
        } else if(nonShared2[i].second ==3){
            _neutral.push_back(nonShared2[i].first);
        }
    }
}
MobGenetics::MobGenetics(MobGenetics *t1, MobGenetics *t2, float factor){
    _cold = (int)(t1->_cold*factor+t2->_cold*(1-factor));
    _hot =(int)(t1->_hot*factor+t2->_hot*(1-factor));
    _humidity = (int)(t1->_humidity*factor+t2->_humidity*(1-factor));
    _health = (int)(t1->_health*factor+t2->_health*(1-factor));
    _reproduceFactor = (int)(t1->_reproduceFactor*factor+t2->_reproduceFactor*(1-factor));
    _strenghtGen =(int)(t1->_strenghtGen*factor+t2->_strenghtGen*(1-factor));
    _distanceMaxMove =(int)(t1->_distanceMaxMove*factor+t2->_distanceMaxMove*(1-factor));
    _distanceMaxReproduce =(int)(t1->_distanceMaxReproduce*factor+t2->_distanceMaxReproduce*(1-factor));
    _strenght = (int)(t1->_strenght*factor+t2->_strenght*(1-factor));
    _jump = (int)(t1->_jump*factor+t2->_jump*(1-factor));
    _speed = (int)(t1->_speed*factor+t2->_speed*(1-factor));
    _atackSpeed = (int)(t1->_atackSpeed*factor+t2->_atackSpeed*(1-factor));
    _foodNeeds =(int)(t1->_foodNeeds*factor+t2->_foodNeeds*(1-factor));
    _age = (int)(t1->_age*factor+t2->_age*(1-factor));
    mixRacePreferences(t1,t2,factor);
    int amounts[14] = {_cold,_hot,_humidity,_health,_reproduceFactor,_strenghtGen,_distanceMaxMove,_distanceMaxReproduce,_strenght,_jump,_speed,_atackSpeed,_foodNeeds, _age};
    valance(amounts);
    _cold = amounts[0];
    _hot = amounts[1];
    _humidity = amounts[2];
    _health = amounts[3];
    _reproduceFactor = amounts[4];
    _strenghtGen = amounts[5];
    _distanceMaxMove = amounts[6];
    _distanceMaxReproduce = amounts[7];
    _strenght = amounts[8];
    _jump = amounts[9];
    _speed = amounts[10];
    _atackSpeed = amounts[11];
    _foodNeeds = amounts[12];
    _age = amounts[13];
    setRelatedFactors();
    mutate();

}
void MobGenetics::mutateRaces(){
    if(rand() % 2 ==0){
        int from = rand() % 4;
        int dest = rand() % 4;
        std::vector<int>* fromVector;
        std::vector<int>* destVector;
        if(from==0) fromVector = &_enemys;
        else if(from==1) fromVector = &_friends;
        else if(from==2) fromVector = &_food;
        else if(from==3) fromVector = &_neutral;
        if(dest==0) destVector = &_enemys;
        else if(dest==1) destVector = &_friends;
        else if(dest==2) destVector = &_food;
        else if(dest==3) destVector = &_neutral;
        if(!fromVector->empty()){
            destVector->push_back((*fromVector)[rand()%fromVector->size()]);
        }
    }
}
void MobGenetics::mutate(){

    int *atribute1, *atribute2;
    int atribute1ind = rand() % 14;
    int atribute2ind = rand() % 14;
    if(atribute1ind==atribute2ind) atribute2ind = atribute2ind % 14;
    switch(atribute1ind){
        case 0:
            atribute1 = &_cold;break;
        case 1:
            atribute1 = &_hot;break;
        case 2:
            atribute1 = &_humidity;break;
        case 3:
            atribute1 = &_health;break;
        case 4:
            atribute1 = &_reproduceFactor;break;
        case 5:
            atribute1 = &_strenghtGen;break;
        case 6:
            atribute1 = &_distanceMaxMove;break;
        case 7:
            atribute1 = &_distanceMaxReproduce;break;
        case 8:
            atribute1 = &_strenght;break;
        case 9:
            atribute1 = &_jump;break;
        case 10:
            atribute1 = &_speed;break;
        case 11:
            atribute1 = &_atackSpeed;break;
        case 12:
            atribute1 = &_foodNeeds;break;
        case 13:
            atribute1 = &_age;break;
        default:atribute1 = &_cold;
    }
    switch(atribute2ind){
        case 0:
            atribute2 = &_cold;break;
        case 1:
            atribute2 = &_hot;break;
        case 2:
            atribute2 = &_humidity;break;
        case 3:
            atribute2 = &_health;break;
        case 4:
            atribute2 = &_reproduceFactor;break;
        case 5:
            atribute2 = &_strenghtGen;break;
        case 6:
            atribute2 = &_distanceMaxMove;break;
        case 7:
            atribute2 = &_distanceMaxReproduce;break;
        case 8:
            atribute2 = &_strenght;break;
        case 9:
            atribute2 = &_jump;break;
        case 10:
            atribute2 = &_speed;break;
        case 11:
            atribute2 = &_atackSpeed;break;
        case 12:
            atribute2 = &_foodNeeds;break;
        case 13:
            atribute2 = &_age;break;
        default:atribute2 = &_cold;break;
    }
    int amount1 = (rand() % 50);
    amount1 = std::min(*atribute1+amount1,100)-*atribute1;
    amount1 = std::min(std::max(*atribute2-amount1,0)+*atribute2,amount1);
    *atribute1 +=amount1;
    *atribute2 -=amount1;
    mutateRaces();
    setRelatedFactors();

}
void MobGenetics::valance(int (&amounts)[14]){
    int sum =0;
    for(int i=0;i<14; i++){
        sum = sum + amounts[i];
    }
    if(sum > 700){
        int diference = sum-700;
        sum = 0;
        for(int i=0;i<14; i++){
            amounts[i] = std::max(0,amounts[i]-diference/14);
            sum = sum + amounts[i];
        }
    }
    while(sum > 700){
        int index = std::rand() % 14;
        if(amounts[index] >0){
            sum = sum-1;
            amounts[index] -= 1;
        }
    }
    if(sum < 700){
        int diference = 700-sum;
        sum = 0;
        for(int i=0;i<14; i++){
            amounts[i] = std::min(100,amounts[i]+diference/14);
            sum = sum + amounts[i];
        }
    }
    while(sum < 700){
        int index = std::rand() % 14;
        if(amounts[index] <100){
            sum = sum+1;
            amounts[index] += 1;
        }
    }
}
void MobGenetics::setRelatedFactors(){
    _size = (_strenght + 100-_speed) /2;
}
