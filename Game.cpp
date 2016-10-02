#include <stdio.h>
#include <math.h>
// TODO: reference additional headers your program requires here
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <iostream>
#include <cassert>


#include "Game.h"


void Game::Start(void)
{

    if(_gameState != Uninitialized)
        return;

    window.create(sf::VideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,32),"Genland!");
    sf::View viewPlayer(sf::FloatRect(200, 200, 1024, 768));

    window.setView(viewPlayer);
    //Game::window sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML works!");

    //PlayerPaddle *player1 = new PlayerPaddle();
    //player1->SetPosition((SCREEN_WIDTH/2),700);

    _gameState= Game::Playing;
    sf::Clock clock;
    sf::Clock clock2;
    float lastTime = 0;

    char c[10];
    sf::Text text;
    sf::Font font;
    text.setCharacterSize(24);
    text.setColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    sf::String str("no data");
    text.setString(str);

    if (!font.loadFromFile("resources/font1.ttf"))
    {
        std::cout << "font error" << std::endl;
    }
    text.setFont(font); // font is a sf::Font
    float fps_timer=0;
    int fps_count=0;
    int fps_count2=0;








    Game::player.Load("blue.png");
    Game::player.SetPosition(0,0);
    Game::player.SetSize(32);

    while(!IsExiting())
    {
        viewPlayer.setCenter(player.GetPosition().x+(player.GetWidth()/2), player.GetPosition().y+(player.GetHeight()/2));
        window.setView(viewPlayer);

        double delta =  clock.restart().asSeconds();


        float currentTime = clock2.restart().asSeconds();
        float fps = 1.f / currentTime;
        fps_count += 1;
        fps_count2 += fps;
        //fps = roundf(fps/10)*10;
        lastTime = currentTime;
        //std::cout << fps << std::endl;

        window.clear(sf::Color(200,200,0));
        GameLoop(delta);
        fps_timer += lastTime;
        sf::View currentView = window.getView();
        sf::Vector2f centerView = currentView.getCenter();
        sf::Vector2f sizeView = currentView.getSize();
        text.setPosition(centerView.x-sizeView.x/2, centerView.y-sizeView.y/2);
        if(fps_timer > 1){
            sprintf(c, "%i", fps_count2/fps_count);
            std::string string(c);
            sf::String str(string);
            text.setString(str);

            fps_count = 0;
            fps_count2 = 0;
            fps_timer = 0;
        }

        window.draw(text);
        window.display();
        //sf::sleep(sf::milliseconds(30));
    }

    window.close();
}

bool Game::IsExiting()
{
    if(_gameState == Game::Exiting)
        return true;
    else
        return false;
}




void Game::GameLoop(double delta)
{
    sf::Event currentEvent;
    //_mainWindow.GetEvent(currentEvent);
    
    if (!rand()%10000)
        exit();


    switch(_gameState)
    {
        case Game::ShowingMenu:
        {
            //ShowMenu();
            break;
        }
        case Game::Playing:
        {


            //_gameObjectManager.UpdateAll();
            //_gameObjectManager.DrawAll(_mainWindow);
            Game::inputs.Update();
            Game::map_curr.UpdateAll(delta);
            Game::player.Update(delta, Game::map_curr, Game::inputs, window, backgrounds);
            backgrounds[0].Draw(window);
            backgrounds[1].Draw(window);
            backgrounds[2].Draw(window);
            backgrounds[3].Draw(window);
            Game::map_curr.DrawMap(window);
            srand(time(NULL));
            Game::player.Draw(window);
            Game::map_curr.DrawFrontItems(window);
            Game::player.DrawInventory(window);
            while(window.pollEvent(currentEvent))
            {
                if (currentEvent.type == sf::Event::Closed ||
                    ((currentEvent.type == sf::Event::KeyPressed) &&
                     (currentEvent.key.code == sf::Keyboard::Escape)))
                {
                    std::cout << "bye" << std::endl;
                    ExitGame();
                    //window.close();
                }
            }


            break;

        }
    }
}



void Game::ExitGame()
{
    _gameState = Exiting;
}

Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::window;
//sf::Vector2i v1(0,0);
//Chunk Game::chunk(v1);
Map Game::map_curr(-1);
Player Game::player;
Inputs Game::inputs;
Background background1("resources/custom_back0.png",1.1, sf::Vector2f(755,2048));
Background background2("resources/custom_back1.png",1.2, sf::Vector2f(755,2048));
Background background3("resources/custom_back2.png",1.3, sf::Vector2f(755,2048));
Background background4("resources/custom_back3.png",1.4, sf::Vector2f(755,2048));
std::vector<Background> Game::backgrounds ={background1,background2,background3,background4};
