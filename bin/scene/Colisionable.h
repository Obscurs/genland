//
// Created by arnito on 17/10/17.
//

#ifndef GENLAND_COLISIONABLE_H
#define GENLAND_COLISIONABLE_H



class Colisionable {
public:
    Colisionable();
    sf::Vector2f evalCollisions(sf::Vector2f posOld, sf::Vector2f posNew, sf::Vector2f size);
    ~Colisionable();
protected:
    int col_bottom;
    int col_top;
    int col_left;
    int col_right;
    float col_bottom_dist;
    float col_top_dist;
    float col_left_dist;
    float col_right_dist;
    void FixColision(sf::Vector2f pos1, sf::Vector2f size1, sf::Vector2f pos2, sf::Vector2f size2);
};


#endif //GENLAND_COLISIONABLE_H
