//
// Created by arnito on 13/12/17.
//

#include <iostream>
#include "Mouse.h"
#include "Resources.h"
#include "scene/Scene.h"

std::map<std::string, int> Mouse::_cursors;
int Mouse::_currentCursor;
int Mouse::_col;
sf::Sprite Mouse::_cursor;
void Mouse::load() {

    static bool firstTimeLoadingResources = true;
    if(! firstTimeLoadingResources) {
        std::cout << "Already loaded, aborting load" << std::endl;
        return;
    }
    firstTimeLoadingResources = false;
    _cursors["default"] = 0;
    _cursors["pick"] = 1;
    _cursors["sword"] = 2;
    _cursor.setTexture(*Resources::getTexture("cursors"));
    _currentCursor = 0;
    _col = 0;
    _cursor.setTextureRect(sf::IntRect(SIZE_SPRITE*_col,SIZE_SPRITE*_currentCursor,SIZE_SPRITE,SIZE_SPRITE));
    std::cout << " Cursors loaded " << std::endl;
}
void Mouse::setScale(float scale){
    _cursor.setScale(sf::Vector2f(scale,scale));
}
void Mouse::setCursor(std::string key, int anim){
    _currentCursor = _cursors[key];
    _col = anim;
    _cursor.setTextureRect(sf::IntRect(SIZE_SPRITE*_col,SIZE_SPRITE*_currentCursor,SIZE_SPRITE,SIZE_SPRITE));
}

void Mouse::draw(sf::RenderWindow &window){
    sf::Vector2f position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    _cursor.setPosition(position);
    window.setMouseCursorVisible(false);
    window.draw(_cursor);
}