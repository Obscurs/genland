//
// Created by arnito on 6/10/17.
//

#include "Terminal.h"
#include "Inputs.h"
#include "Resources.h"
#include "Debuger.h"
#include <iostream>
#include <fstream>
#include <sstream>
Terminal::Terminal():
    _instructionBox(sf::Vector2f(850,700),
                    sf::Vector2f(1000,40),
                    0,
                    "", ""){
    _active = false;
}
Terminal::~Terminal(){

}
void Terminal::sendTerminalInstruction(){
    std::string instruction = _instructionBox.getText();
    _instructionBox.setText("");
    _commands.push_back(instruction);
    Debuger::interpInstruction(instruction);
}
void Terminal::setText(std::string s){
    _instructionBox.setText(s);
}
std::string Terminal::getText(){
    return _instructionBox.getText();
}
bool Terminal::isActive(){
    return _active;
}
void Terminal::Update(){
    if(Inputs::KeyBreak(Inputs::F1)) {
        _active = !_active;
    }
    if(_active){
        _instructionBox.update(sf::Vector2f(0,0));
    }
}
void Terminal::draw(sf::RenderWindow &_window, sf::Text &text){
    if(_active){
        sf::View currentView    = _window.getView();
        sf::Vector2f centerView = currentView.getCenter();
        sf::Vector2f sizeView   = currentView.getSize();

        for(int i=std::max(int(_commands.size())-20,0); i<_commands.size();++i){
            text.setPosition(centerView.x-sizeView.x/2, centerView.y-sizeView.y/2+200+(i-std::max(int(_commands.size())-20,0))*20);
            text.setString(_commands[i]);
            _window.draw(text);
        }

        _instructionBox.position = sf::Vector2f(centerView.x-sizeView.x/2+10, centerView.y-sizeView.y/2+800);
        _instructionBox.Draw(_window,*Resources::getFont("debugFont"));
    }
}