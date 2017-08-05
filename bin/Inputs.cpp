#include <stdio.h>


// TODO: reference additional headers your program requires here
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <map>
#include <iostream>
#include <cassert>
#include <cstring>


#include "Inputs.h"

const sf::Keyboard::Key Inputs::_keyMap[NUM_KEYS] = {
        sf::Keyboard::Up,
        sf::Keyboard::Down,
        sf::Keyboard::Left,
        sf::Keyboard::Right,
        sf::Keyboard::LShift,
        sf::Keyboard::Z,
        sf::Keyboard::X,
        sf::Keyboard::Return,
        sf::Keyboard::BackSpace,
        sf::Keyboard::LControl,
        sf::Keyboard::Tab,
        sf::Keyboard::A,
        sf::Keyboard::D,
        sf::Keyboard::Q,
        sf::Keyboard::Space,
        sf::Keyboard::Num0,
        sf::Keyboard::Num1,
        sf::Keyboard::Num2,
        sf::Keyboard::Num3,
        sf::Keyboard::Num4,
        sf::Keyboard::Num5,
        sf::Keyboard::Num6,
        sf::Keyboard::Num7,
        sf::Keyboard::Num8,
        sf::Keyboard::Num9,
        sf::Keyboard::Escape,
        sf::Keyboard::F1,
        sf::Keyboard::F2,
        sf::Keyboard::F3,
        sf::Keyboard::F4,
        sf::Keyboard::F5,
        sf::Keyboard::F6,
        sf::Keyboard::Add,
        sf::Keyboard::Subtract,
        sf::Keyboard::M

};
const sf::Mouse::Button Inputs::_mouseMap[NUM_BUTTONS] = {
        sf::Mouse::Left,
        sf::Mouse::Right


};
int Inputs::wheelDelta = 0;
bool Inputs::_lastKeyState[Inputs::NUM_KEYS] = {0};
bool Inputs::_currentKeyState[Inputs::NUM_KEYS] = {0};
bool Inputs::_lastButtonState[Inputs::NUM_BUTTONS] = {0};
bool Inputs::_currentButtonState[Inputs::NUM_BUTTONS] = {0};

void Inputs::Update() {
    // update _lastKeyState
    memcpy(_lastKeyState, _currentKeyState, sizeof(_currentKeyState));
    memcpy(_lastButtonState, _currentButtonState, sizeof(_currentButtonState));
    wheelDelta = 0;

}

void Inputs::KeyPressed(const sf::Keyboard::Key &code) {

    // update _currentKeyState
    unsigned int i;
    for (i = 0; i < NUM_KEYS; ++i) {
        Inputs::Key key = (Inputs::Key) i;
        sf::Keyboard::Key keyboard_key = _keyMap[key];
        if(code == keyboard_key) {
            _currentKeyState[key] = true;
        }

    }
}
void Inputs::KeyReleased(const sf::Keyboard::Key &code) {

    // update _currentKeyState
    unsigned int i;
    for (i = 0; i < NUM_KEYS; ++i) {
        Inputs::Key key = (Inputs::Key) i;
        sf::Keyboard::Key keyboard_key = _keyMap[key];
        if(code == keyboard_key) {
            _currentKeyState[key] = false;
        }

    }
}

void Inputs::ButtonPressed(const sf::Mouse::Button &code) {

    // update _currentKeyState
    unsigned int i;
    for (i = 0; i < NUM_KEYS; ++i) {
        Inputs::Button key = (Inputs::Button) i;
        sf::Mouse::Button keyboard_key = _mouseMap[key];
        if(code == keyboard_key) {
            _currentButtonState[key] = true;
        }

    }
}
void Inputs::ButtonReleased(const sf::Mouse::Button &code) {

    // update _currentKeyState
    unsigned int i;
    for (i = 0; i < NUM_KEYS; ++i) {
        Inputs::Button key = (Inputs::Button) i;
        sf::Mouse::Button keyboard_key = _mouseMap[key];
        if(code == keyboard_key) {
            _currentButtonState[key] = false;
        }

    }
}

bool Inputs::KeyDown(Inputs::Key k) { return _currentKeyState[k]; }
bool Inputs::KeyUp(Inputs::Key k)   { return not _currentKeyState[k]; }

bool Inputs::KeyHit(Inputs::Key k) {
    return not _lastKeyState[k] and _currentKeyState[k];
}

bool Inputs::KeyBreak(Inputs::Key k) {
    return _lastKeyState[k] and not _currentKeyState[k];
}

bool Inputs::MouseDown(Inputs::Button k) { return _currentButtonState[k]; }
bool Inputs::MouseUp(Inputs::Button k)   { return not _currentButtonState[k]; }

bool Inputs::MouseHit(Inputs::Button k) {
    return not _lastButtonState[k] and _currentButtonState[k];
}

bool Inputs::MouseBreak(Inputs::Button k) {
    return _lastButtonState[k] and not _currentButtonState[k];
}



void Inputs::UpdateWheel(int delta) {
	wheelDelta = delta;
}
int Inputs::GetWheel() {
    return wheelDelta;
}


