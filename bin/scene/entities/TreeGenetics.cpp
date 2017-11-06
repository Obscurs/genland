//
// Created by arnito on 18/09/17.
//

#include <cstdlib>
#include <algorithm>
#include <iostream>
#include "TreeGenetics.h"
TreeGenetics::TreeGenetics(){
    int amounts[6] = {rand() % 100};
    valance(amounts);
    _cold = amounts[0];
    _hot = amounts[1];
    _humidity = amounts[2];
    _health = amounts[3];
    _reproduceFactor = amounts[4];
    _strenghtGen = amounts[5];

    _corb = rand() % 3 +1;
    _amountLeave = rand() % 3 +1;
    _typeLeave = rand() % 3 +1;
    _branchAmount = float(rand() % 70+10)/100;
    _sizeBranch = float(rand() % 90+10)/100;
    _curveBranch = float(rand() % 80+10)/100;
    setRelatedFactors();



}
TreeGenetics::TreeGenetics(TreeGenetics *t1, TreeGenetics *t2, float factor){
    _cold = (int)(t1->_cold*factor+t2->_cold*(1-factor));
    _hot =(int)(t1->_hot*factor+t2->_hot*(1-factor));
    _humidity = (int)(t1->_humidity*factor+t2->_humidity*(1-factor));
    _health = (int)(t1->_health*factor+t2->_health*(1-factor));
    _reproduceFactor = (int)(t1->_reproduceFactor*factor+t2->_reproduceFactor*(1-factor));
    _strenghtGen =(int)(t1->_strenghtGen*factor+t2->_strenghtGen*(1-factor));

    _corb = (int)(float(t1->_corb)*factor+float(t2->_corb)*(1-factor));
    _amountLeave = (int)(float(t1->_amountLeave)*factor+float(t2->_amountLeave)*(1-factor));
    _typeLeave = (int)(float(t1->_typeLeave)*factor+float(t2->_typeLeave)*(1-factor));
    _branchAmount = t1->_branchAmount*factor+t2->_branchAmount*(1-factor);
    _sizeBranch = t1->_sizeBranch*factor+t2->_sizeBranch*(1-factor);
    _curveBranch = t1->_curveBranch*factor+t2->_curveBranch*(1-factor);

    int amounts[6] = {_cold,_hot,_humidity,_health,_reproduceFactor,_strenghtGen};
    valance(amounts);
    _cold = amounts[0];
    _hot = amounts[1];
    _humidity = amounts[2];
    _health = amounts[3];
    _reproduceFactor = amounts[4];
    _strenghtGen = amounts[5];
    setRelatedFactors();
    mutate();
}
void TreeGenetics::mutate(){
    int atribute = rand() % 6;
    float amount = float(rand() % 100)/100-0.5f;
    switch(atribute){
        case 0:
            if(amount>=0) _corb = std::min(3, _corb+1);
            else _corb = std::max(1, _corb-1);
            break;
        case 1:
            if(amount>=0) _amountLeave = std::min(3, _amountLeave+1);
            else _amountLeave = std::max(1, _amountLeave-1);
            break;
        case 2:
            if(amount>=0) _typeLeave = std::min(3, _typeLeave+1);
            else _typeLeave = std::max(1, _typeLeave-1);
            break;
        case 3:
            if(amount>=0) _branchAmount = std::min(0.8f, _branchAmount+amount);
            else _branchAmount = std::max(0.1f, _branchAmount+amount);
            break;
        case 4:
            if(amount>=0) _sizeBranch = std::min(1.f, _sizeBranch+amount);
            else _sizeBranch = std::max(0.1f, _sizeBranch+amount);
            break;
        case 5:
            if(amount>=0) _curveBranch = std::min(0.9f, _curveBranch+amount);
            else _curveBranch = std::max(0.1f, _curveBranch+amount);
            break;
        default:
            break;
    }
    int *atribute1, *atribute2;
    int atribute1ind = rand() % 6;
    int atribute2ind = rand() % 6;
    if(atribute1ind==atribute2ind) atribute2ind = atribute2ind % 6;
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
        default:atribute2 = &_cold;break;
    }
    int amount1 = (rand() % 50);
    amount1 = std::min(*atribute1+amount1,100)-*atribute1;
    amount1 = std::min(std::max(*atribute2-amount1,0)+*atribute2,amount1);
    *atribute1 +=amount1;
    *atribute2 -=amount1;
    setRelatedFactors();
}
void TreeGenetics::valance(int (&amounts)[6]){
    int sum =0;
    for(int i=0;i<6; i++){
        sum = sum + amounts[i];
    }
    if(sum > 300){
        int diference = sum-300;
        sum = 0;
        for(int i=0;i<6; i++){
            amounts[i] = std::max(0,amounts[i]-diference/6);
            sum = sum + amounts[i];
        }
    }
    while(sum > 300){
        int index = std::rand() % 6;
        if(amounts[index] >0){
            sum = sum-1;
            amounts[index] -= 1;
        }
    }
    if(sum < 300){
        int diference = 300-sum;
        sum = 0;
        for(int i=0;i<6; i++){
            amounts[i] = std::min(100,amounts[i]+diference/6);
            sum = sum + amounts[i];
        }
    }
    while(sum < 300){
        int index = std::rand() % 6;
        if(amounts[index] <100){
            sum = sum+1;
            amounts[index] += 1;
        }
    }
}
void TreeGenetics::setRelatedFactors(){
    if((_cold >70 || _health > 70) && _reproduceFactor <40) _height = 3;
    else if((_cold >30 || _health > 50) && _reproduceFactor <60) _height = 2;
    else _height = 1;


    if(_hot >70 || _strenghtGen < 30) _amplitude = 1;
    else if(_hot >40) _amplitude = 2;
    else _amplitude = 3;


    if(_humidity >70 || _reproduceFactor > 70) _densityLeave = 3;
    else if(_humidity >40 || _reproduceFactor > 60) _densityLeave = 2;
    else _densityLeave = 1;
}

