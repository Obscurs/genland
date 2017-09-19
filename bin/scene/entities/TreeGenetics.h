//
// Created by arnito on 18/09/17.
//

#ifndef GENLAND_TREEGENETICS_H
#define GENLAND_TREEGENETICS_H


class TreeGenetics {
public:
    TreeGenetics();
    TreeGenetics(TreeGenetics *t1, TreeGenetics *t2, float factor);
    //branch
    float _branchAmount;
    float _sizeBranch;
    float _curveBranch;

    //core
    int _amplitude;
    int _height;
    int _corb;

    //leave
    int _densityLeave;
    int _amountLeave;
    int _typeLeave;

    //Resistance
    int _cold;      //-> alture
    int _hot;       //-> amplitude
    int _humidity;  //-> density

    //GenFactors
    int _health;
    int _reproduceFactor;
    int _strenghtGen;
    void mutate();
private:
    void valance(int (&amounts)[6]);
    void setRelatedFactors();
};


#endif //GENLAND_TREEGENETICS_H
