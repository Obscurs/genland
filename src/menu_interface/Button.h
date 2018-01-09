//
// Created by ant on 03/11/16.
//

#ifndef GENLAND_BUTTON_H
#define GENLAND_BUTTON_H
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

class Button {
public:
    Button(sf::Vector2f pos, sf::Vector2f s, std::string txt, int tp);
    ~Button();
    sf::Vector2f position;
    sf::Vector2f size;
    std::string text;
    int type;
    bool mouseOver;
    void Draw(sf::RenderWindow &window,sf::Font font);
    void update(sf::Vector2f mousePos);
};


#endif //GENLAND_BUTTON_H
