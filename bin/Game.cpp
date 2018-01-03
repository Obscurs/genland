#include <stdio.h>
#include <math.h>
// TODO: reference additional headers your program requires here
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>


#include <fstream>
#include <sys/stat.h>
#include <string.h>


#include "Debuger.h"
#include "Game.h"
#include "Settings.h"
#include "Inputs.h"
#include "functions.h"
#include "SoundManager.hpp"
#include "Mouse.h"


Game::Game():
        _window(),
        _backgrounds(true)
{
    _gameState = Uninitialized;
    _true_exit=false;
    _timerMusic =0;
    _counter = 0;
    if (!_font.loadFromFile("resources/font1.ttf"))
    {
        std::cout << "font error" << std::endl;
    }


}
Game::~Game(){

}

void Game::LoadData(){

    std::vector<std::string> save_folders;
    GetFilesInDirectory(save_folders, "save");
    MenuLoadGame::save_list.elements.clear();
    for(int i = 0; i<save_folders.size(); i++){
        std::string route = save_folders[i];
        route.append("/data");
        if(exists_file(route)) {
            std::ifstream myfile(route);
            std::string data_seed= "def", data_name = "def", data_day = "def";
            myfile >> data_seed >> data_name >> data_day;
            int len = data_day.length();
            std::string aux = "";
            for(int j = 5; j>len; --j){
                aux.append("0");
            }
            aux.append(data_day);
            MenuLoadGame::save_list.insertElement("Day: "+ aux +"   |   "+data_name);
            myfile.close();
        }
        else std::cout << "ERROR: could not read file " << route << std::endl;

    }
}
void Game::Start(void)
{
    if(_gameState != Uninitialized) return;

    LoadData();
    _window.create(sf::VideoMode(Settings::SCREEN_WIDTH,Settings::SCREEN_HEIGHT,32),"Genland!");
    _gameState= Game::ShowingMenu;
    Debuger::Init(_window);
    MenuMain::view = MagicView(&_window,MagicView::expanded,sf::Vector2i(2000,2000));
    NewGameMenu::view = MagicView(&_window,MagicView::expanded,sf::Vector2i(2000,2000));
    MenuLoadGame::view = MagicView(&_window,MagicView::expanded,sf::Vector2i(2000,2000));
    MenuConfigGame::view = MagicView(&_window,MagicView::expanded,sf::Vector2i(2000,2000));
    Scene* scene = Scene::getScene();
    if(scene->isInit()) scene->updateView();
    MenuMain::view.update();
    MenuConfigGame::view.update();
    MenuLoadGame::view.update();
    NewGameMenu::view.update();
    while(!IsExiting())
    {
        _window.clear(sf::Color(0,255,0));
        GameLoop();
        Debuger::Draw();
        _window.display();
    }

    _window.close();
}

bool Game::IsExiting()
{
    if(_gameState == Game::Exiting)
        return true;
    else
        return false;
}



void Game::Events(){
    sf::Event currentEvent;
    while(_window.pollEvent(currentEvent))
    {
        if(currentEvent.type == sf::Event::MouseWheelMoved)
        {
            Inputs::UpdateWheel(currentEvent.mouseWheel.delta);
        }
        else if (currentEvent.type == sf::Event::Resized){
            std::cout << "res" << std::endl;
            Scene* scene = Scene::getScene();
            if(scene->isInit()) scene->updateView();
            MenuMain::view.update();
            MenuConfigGame::view.update();
            MenuLoadGame::view.update();
            NewGameMenu::view.update();
        }
        else if (currentEvent.type == sf::Event::Closed)
        {
            _true_exit=true;
            Game::ExitGame();
        } if (currentEvent.type == sf::Event::KeyPressed) {
            Inputs::KeyPressed(currentEvent.key.code);
        }
        else if (currentEvent.type == sf::Event::KeyReleased) {
            Inputs::KeyReleased(currentEvent.key.code);
        } else if (currentEvent.type == sf::Event::MouseButtonPressed) {
            Inputs::ButtonPressed(currentEvent.mouseButton.button);
        }
        else if (currentEvent.type == sf::Event::MouseButtonReleased) {
            Inputs::ButtonReleased(currentEvent.mouseButton.button);
        }  if(currentEvent.type == sf::Event::TextEntered && _gameState == Game::NewGame)
        {
            if(((currentEvent.text.unicode < 58 && currentEvent.text.unicode >= 48) || currentEvent.text.unicode == 8 ) && NewGameMenu::seed.selected)
            {
                std::string name = NewGameMenu::seed.getText();
                if( currentEvent.text.unicode == 13 ) // return key
                {
                    // finished entering name
                } else if( currentEvent.text.unicode == 8 ) { // backspace
                    if( name.size() > 0 ) name.resize( name.size() - 1 );
                } else if(name.size()<10){
                    name += static_cast<char>(currentEvent.text.unicode);
                }
                NewGameMenu::seed.setText( name );
            }
            else if(((currentEvent.text.unicode < 128 && currentEvent.text.unicode >= 48) || currentEvent.text.unicode == 8 )&& NewGameMenu::name.selected)
            {
                std::string name = NewGameMenu::name.getText();
                if( currentEvent.text.unicode == 13 ) // return key
                {
                    // finished entering name
                } else if( currentEvent.text.unicode == 8 ) { // backspace
                    if( name.size() > 0 ) name.resize( name.size() - 1 );
                } else if(name.size()<10){
                    name += static_cast<char>(currentEvent.text.unicode);
                }
                NewGameMenu::name.setText( name );
            }
        }else if(currentEvent.type == sf::Event::TextEntered && _gameState == Game::Playing && Debuger::isTerminalActive())
        {
            if(((currentEvent.text.unicode < 128 && currentEvent.text.unicode >= 48) || currentEvent.text.unicode == 8  || currentEvent.text.unicode == 32 || currentEvent.text.unicode == 13))
            {
                std::string instruction = Debuger::getTerminalText();
                if( currentEvent.text.unicode == 13 ) // return key
                {
                    Debuger::sendTerminalInstruction( );
                } else if( currentEvent.text.unicode == 8 ) { // backspace
                    if( instruction.size() > 0 ) {
                        instruction.resize( instruction.size() - 1 );
                    }
                    Debuger::setTerminalText( instruction );
                } else if(instruction.size()<64){
                    instruction += static_cast<char>(currentEvent.text.unicode);
                    Debuger::setTerminalText( instruction );
                } else {
                    Debuger::setTerminalText( instruction );
                }

            }
        }
    }
}
void Game::GameLoop()
{
    if(_timerMusic<=0){
        SoundManager::playMusicNoRestart("ambient1");
        _timerMusic = (std::rand()%3+1)*60;
    }

    Scene* scene = Scene::getScene();
    sf::Time deltatime = _clock.restart();
    Debuger::Update(deltatime);
    _timerMusic -= deltatime.asSeconds();
    float delta = deltatime.asSeconds();
    Inputs::Update();
    Events();
    Mouse::setCursor("default",0);
    Mouse::setScale(1.f);
    switch(_gameState)
    {
        case Game::Playing:
        {
            Mouse::setScale(0.3f);
            if(Inputs::KeyBreak(Inputs::ESC)) _gameState = Game::ShowingMenu;
            Debuger::InsertClockMark("PreLoop");
            scene->update(_window,delta);
            Debuger::InsertClockMark("update");
            scene->draw(_window);
            Debuger::InsertClockMark("draw");


            break;
        }
        case Game::ShowingMenu:
        {
            _counter +=delta*100;
            _backgrounds.Update(sf::Vector2f(_counter,-2000),0);
            if(Inputs::KeyBreak(Inputs::ESC) && scene->isInit()) _gameState = Game::Playing;
            if(MenuMain::newGameClicked()) _gameState = NewGame;
            else if(MenuMain::exitClicked()) {
                _true_exit=true;
                ExitGame();
            }
            else if(MenuMain::loadClicked()) _gameState = LoadGame;
            else if(MenuMain::configClicked()) _gameState = Config;
            MenuMain::Update();
            sf::View v = _window.getView();
            sf::View v2 = _window.getView();
            v2.setCenter(sf::Vector2f(v.getCenter().x,v.getCenter().y-2000));
            _window.setView(v2);
            _backgrounds.Draw(_window);
            _window.setView(v);
            MenuMain::Draw(_window, _font);

            break;
        }
        case Game::NewGame:
        {
            _counter +=delta*100;
            _backgrounds.Update(sf::Vector2f(_counter,-2000),0);
            if(NewGameMenu::backClicked()) _gameState = ShowingMenu;
            else if(NewGameMenu::seedClicked()){
                NewGameMenu::disSelectAll();
                NewGameMenu::seed.selected = true;
            }
            else if(NewGameMenu::nameClicked()){
                NewGameMenu::disSelectAll();
                NewGameMenu::name.selected = true;
            }
            else if(NewGameMenu::startClicked() && NewGameMenu::seed.text != "") {
                std::vector<std::string> out;
                GetFilesInDirectory(out,"save");
                if(out.size()<10) {


                    std::string new_game_path = "save/s";
                    if (out.size() < 10) new_game_path.append("0");
                    new_game_path.append(std::to_string(out.size()));
                    CreateNewGame(new_game_path, NewGameMenu::seed.getText(), NewGameMenu::name.getText());
                    scene->init(new_game_path, _window, NewGameMenu::seed.getText());

                    _gameState = Playing;
                } else std::cout << "demasiadas partidas guardadas" << std::endl;
            }


            NewGameMenu::Update(delta);
            sf::View v = _window.getView();
            sf::View v2 = _window.getView();
            v2.setCenter(sf::Vector2f(v.getCenter().x,v.getCenter().y-2000));
            _window.setView(v2);
            _backgrounds.Draw(_window);
            _window.setView(v);
            NewGameMenu::Draw(_window, _font);
            break;
        }
        case Game::LoadGame:
        {

            _counter +=delta*100;
            _backgrounds.Update(sf::Vector2f(_counter,-2000),0);
            if(MenuLoadGame::backClicked()) _gameState = ShowingMenu;

            else if(MenuLoadGame::loadClicked() && !MenuLoadGame::save_list.elements.empty()) {

                std::string folder_save = "s";
                int slot = MenuLoadGame::save_list.selected_slot;
                if(slot<10) folder_save.append("0");
                if(slot <10){
                    folder_save.append(std::to_string(slot));
                    std::vector<std::string> out;
                    GetFilesInDirectory(out,"save");
                    std::string new_game_path = "save/";
                    new_game_path.append(folder_save);

                    std::string route = new_game_path;
                    route.append("/data");
                    if(exists_file(route)) {

                        std::string route_aux = route;
                        //route_aux.append(".txt");
                        std::ifstream myfile(route);
                        //myfile.open(route);
                        std::string data_seed= "def", data_name = "def";
                        myfile >> data_seed >> data_name;
                        myfile.close();
                        scene->saveGame();
                        scene->init(new_game_path, _window, data_seed);
                        _gameState = Playing;
                    }
                    else std::cout << "ERROR: could not read file " << route << std::endl;





                } else std::cout << "slot demasiado grande" << std::endl;
            }
            else if(MenuLoadGame::delClicked() && !scene->isInit() && !MenuLoadGame::save_list.elements.empty()) {

                std::string folder_save = "s";
                int slot = MenuLoadGame::save_list.selected_slot;
                if(slot<10) folder_save.append("0");
                if(slot <10){
                    folder_save.append(std::to_string(slot));
                    std::vector<std::string> out;
                    GetFilesInDirectory(out,"save");
                    std::string del_game_path = "save/";
                    del_game_path.append(folder_save);
                    std::cout << "deleting "<< del_game_path << std::endl;
                    DeleteGame(slot, "save");
                    LoadData();
                } else std::cout << "slot demasiado grande" << std::endl;
            }
            MenuLoadGame::Update();
            sf::View v = _window.getView();
            sf::View v2 = _window.getView();
            v2.setCenter(sf::Vector2f(v.getCenter().x,v.getCenter().y-2000));
            _window.setView(v2);
            _backgrounds.Draw(_window);
            _window.setView(v);
            MenuLoadGame::Draw(_window, _font);
            break;

        }

        case Game::Config:
        {
            _counter +=delta*100;
            _backgrounds.Update(sf::Vector2f(_counter,-2000),0);
            if(MenuConfigGame::backClicked() && !MenuConfigGame::resolution_visible) _gameState = ShowingMenu;
            else if(MenuConfigGame::resolution_visible && Inputs::KeyBreak(Inputs::ENTER)){
                MenuConfigGame::resolution_visible = false;
                int x = MenuConfigGame::res_keys[MenuConfigGame::resolution_list.selected_slot*2];
                int y = MenuConfigGame::res_keys[MenuConfigGame::resolution_list.selected_slot*2+1];
                _window.setSize(sf::Vector2u(x,y));
                MenuConfigGame::view.update();

                if(exists_file("config/config")) {
                    std::ofstream myfile;
                    myfile.open("config/config");
                    myfile << x;
                    myfile << "\n";
                    myfile << y;
                    myfile.close();
                }
                _true_exit=false;
                ExitGame();
            }
            else if(MenuConfigGame::resClicked()) {
                MenuConfigGame::resolution_visible = true;


            }

            MenuConfigGame::Update();
            sf::View v = _window.getView();
            sf::View v2 = _window.getView();
            v2.setCenter(sf::Vector2f(v.getCenter().x,v.getCenter().y-2000));
            _window.setView(v2);
            _backgrounds.Draw(_window);
            _window.setView(v);
            MenuConfigGame::Draw(_window, _font);
            break;

        }
        case Game::Uninitialized: {
            break;
        }
        case Game::Exiting: {
            break;
        }

    }
    Mouse::draw(_window);
}



void Game::ExitGame()
{
    Scene* scene = Scene::getScene();
    scene->saveGame();
    _gameState = Exiting;
}
void Game::CreateNewGame(std::string path, std::string seed, std::string name) {
    //GAME FOLDER
    const int dir_err = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == dir_err)
    {
        printf("Error creating directory s !n");
        exit(1);
    }
    //UPDATING LOAD MENU
    MenuLoadGame::save_list.insertElement(name);

    //DATA FILE
    std::string path_data = path;
    path_data.append("/data");
    std::ofstream myfile;
    myfile.open (path_data);
    myfile << seed;
    myfile << "\r\n";
    myfile << name;
    myfile.close();

    //MAP FOLDER
    std::string path_map = path;
    path_map.append("/map");
    const int dir_err2 = mkdir(path_map.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == dir_err2)
    {
        printf("Error creating directory map !n");
        exit(1);
    }
    //ENTITIES FOLDER
    std::string path_entities = path;
    path_entities.append("/entities");
    const int dir_err3 = mkdir(path_entities.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == dir_err3)
    {
        printf("Error creating directory entities !n");
        exit(1);
    }
}

void Game::DeleteGame(int index, std::string path) {
    std::vector<std::string> out;
    GetFilesInDirectory(out,path);
    std::string path_delete = path;
    path_delete.append("/s");
    if(index <10) path_delete.append("0");
    path_delete.append(std::to_string(index));
    DeleteDirectory(path_delete.c_str());
    int result;
    for(int i=index+1; i<out.size();i++){
        std::string path_rename_old = path;
        path_rename_old.append("/s");
        std::string path_rename_new = path_rename_old;
        if(i<10) path_rename_old.append("0");
        path_rename_old.append(std::to_string(i));
        if(i-1<10) path_rename_new.append("0");
        path_rename_new.append(std::to_string(i-1));
        char oldname[50];
        strcpy(oldname, path_rename_old.c_str());
        char newname[50] ="newname.txt";
        strcpy(newname, path_rename_new.c_str());
        result= rename( oldname , newname );
        if ( result == 0 )
            puts ( "File successfully renamed" );
        else
            perror( "Error renaming file" );
    }
}



