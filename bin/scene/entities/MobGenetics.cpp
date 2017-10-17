//
// Created by arnito on 11/10/17.
//

#include "MobGenetics.h"
#include <cstdlib>
#include <algorithm>
#include <iostream>
MobGenetics::MobGenetics(){
    int amounts[8] = {rand() % 100};
    valance(amounts);
    _cold = amounts[0];
    _hot = amounts[1];
    _humidity = amounts[2];
    _health = amounts[3];
    _reproduceFactor = amounts[4];
    _strenghtGen = amounts[5];
    _distanceMaxMove = amounts[6];
    _distanceMaxReproduce = amounts[7];
    setRelatedFactors();
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

    int amounts[8] = {_cold,_hot,_humidity,_health,_reproduceFactor,_strenghtGen,_distanceMaxMove,_distanceMaxReproduce};
    valance(amounts);
    _cold = amounts[0];
    _hot = amounts[1];
    _humidity = amounts[2];
    _health = amounts[3];
    _reproduceFactor = amounts[4];
    _strenghtGen = amounts[5];
    _distanceMaxMove = amounts[6];
    _distanceMaxReproduce = amounts[7];
    setRelatedFactors();
    mutate();
}
void MobGenetics::mutate(){

    int *atribute1, *atribute2;
    int atribute1ind = rand() % 8;
    int atribute2ind = rand() % 8;
    if(atribute1ind==atribute2ind) atribute2ind = atribute2ind % 8;
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
        default:atribute2 = &_cold;break;
    }
    int amount1 = (rand() % 50);
    amount1 = std::min(*atribute1+amount1,100)-*atribute1;
    amount1 = std::min(std::max(*atribute2-amount1,0)+*atribute2,amount1);
    *atribute1 +=amount1;
    *atribute2 -=amount1;
    setRelatedFactors();
}
void MobGenetics::valance(int (&amounts)[8]){
    int sum =0;
    for(int i=0;i<8; i++){
        sum = sum + amounts[i];
    }
    if(sum > 400){
        int diference = sum-400;
        sum = 0;
        for(int i=0;i<8; i++){
            amounts[i] = std::max(0,amounts[i]-diference/8);
            sum = sum + amounts[i];
        }
    }
    while(sum > 400){
        int index = std::rand() % 8;
        if(amounts[index] >0){
            sum = sum-1;
            amounts[index] -= 1;
        }
    }
    if(sum < 400){
        int diference = 400-sum;
        sum = 0;
        for(int i=0;i<8; i++){
            amounts[i] = std::min(100,amounts[i]+diference/8);
            sum = sum + amounts[i];
        }
    }
    while(sum < 400){
        int index = std::rand() % 8;
        if(amounts[index] <100){
            sum = sum+1;
            amounts[index] += 1;
        }
    }
}
void MobGenetics::setRelatedFactors(){

}
