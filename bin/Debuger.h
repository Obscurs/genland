//
// Created by arnito on 25/04/17.
//

#ifndef GENLAND_DEBUGER_H
#define GENLAND_DEBUGER_H
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "Resources.h"
#include "Terminal.h"

class Debuger {
public:
    static void Init(sf::RenderWindow &window);
    static void Update(const sf::Time& deltatime);
    static void Draw();
    static void InsertClockMark(std::string name);
    static const int DISPLACEMENT = 25;
    static bool activated;
    static std::string metric1;
    static std::string metric2;
    static std::string metric3;
    static bool isTerminalActive();
    static void setTerminalText(std::string s);
    static std::string getTerminalText();
    static void sendTerminalInstruction();
    static void interpInstruction(std::string s);
private:

    static std::vector<sf::Time> _clockMarks;
    static std::vector<std::string> _clockNames;
    static sf::Text _text;
    static Terminal _terminal;
    static float _fpsTimer;
    static int _fpsCount;
    static int _fpsCount2;
    static int _displace;
    static int _old_fps;
    static sf::Clock _clock;
    static sf::RenderWindow *_window;
    static bool _is_init;
    static void InitFpsText();
    static void UpdateFpsText(const sf::Time& deltatime);
    static void DrawClockMarks();
    static void DrawFps();
    static void DrawPlayerStats();
    static void DrawWorldStats();
    static void DrawBiomeStats();
};


#endif //GENLAND_DEBUGER_H
