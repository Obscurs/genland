//
// Created by arnito on 11/12/16.
//

#ifndef GENLAND_TEXTINPUT_H
#define GENLAND_TEXTINPUT_H
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

class TextInput {
public:
    TextInput(sf::Vector2f pos, sf::Vector2f s, int tp, std::string text_default, std::string lab);
    ~TextInput();
    sf::Vector2f position;
    sf::Vector2f size;
    std::string text;
    std::string label;
    int type;
    bool mouseOver;
    void Draw(sf::RenderWindow &window,sf::Font font);
    void update(sf::Vector2f mousePos);
    std::string getText();
    void setText(std::string new_text);
    bool selected;
private:
};


#endif //GENLAND_TEXTINPUT_H
