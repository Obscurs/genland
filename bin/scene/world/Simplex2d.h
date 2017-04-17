//
// Created by arnito on 19/08/16.
//

#ifndef GENLAND_SIMPLEX2D_H
#define GENLAND_SIMPLEX2D_H
#include <random>

class Simplex2d {
    public:
        Simplex2d(std::mt19937* generator, float scale, float min, float max);
        ~Simplex2d();
        float valSimplex2D(float x, float y);


    private:
        int fastfloor(const float x);
        float dot(const int* g, const float x, const float y);
        static const int grad3[12][3];
        std::mt19937* generator;

        std::vector<int> perm;
        float scale;
        float min;
        float max;
};


#endif //GENLAND_SIMPLEX2D_H
