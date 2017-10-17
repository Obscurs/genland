//
// Created by arnito on 11/10/17.
//

#ifndef GENLAND_MOBGENETICS_H
#define GENLAND_MOBGENETICS_H


class MobGenetics {
public:
    MobGenetics();
    MobGenetics(MobGenetics *t1, MobGenetics *t2, float factor);

    //Resistance
    int _cold;      //-> alture
    int _hot;       //-> amplitude
    int _humidity;  //-> density

    //GenFactors
    int _health;
    int _reproduceFactor;
    int _strenghtGen;
    int _distanceMaxMove;
    int _distanceMaxReproduce;

    void mutate();
private:
    void valance(int (&amounts)[8]);
    void setRelatedFactors();
};


#endif //GENLAND_MOBGENETICS_H
