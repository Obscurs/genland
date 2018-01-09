//
// Created by arnito on 13/12/17.
//

#ifndef GENLAND_MOUSE_HPP
#define GENLAND_MOUSE_HPP


#include <SFML/Graphics.hpp>
#include <map>

class Mouse {
public:
    static void setCursor(std::string key, int anim);
    static void draw(sf::RenderWindow &window);
    static void load();
    static void setScale(float scale);
    static const int SIZE_SPRITE = 64;
private:
    static std::map<std::string, int> _cursors;
    static sf::Sprite _cursor;
    static int _currentCursor;
    static int _col;

};


#endif //GENLAND_MOUSE_HPP
