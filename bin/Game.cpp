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
#include "functions.h"


Game::Game():
        _window(),
        _scene(_window)
{
    _resize = false;
    _gameState = Uninitialized;
    _true_exit=false;
    _debuger = false;
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
            std::string data_seed= "def", data_name = "def";
            myfile >> data_seed >> data_name;
            MenuLoadGame::save_list.insertElement(data_name);
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
    Debuger::Init(_window, _scene);
    MenuMain::view = MagicView(&_window,MagicView::expanded,sf::Vector2i(2000,2000));
    NewGameMenu::view = MagicView(&_window,MagicView::expanded,sf::Vector2i(2000,2000));
    MenuLoadGame::view = MagicView(&_window,MagicView::expanded,sf::Vector2i(2000,2000));
    MenuConfigGame::view = MagicView(&_window,MagicView::expanded,sf::Vector2i(2000,2000));
    while(!IsExiting())
    {
        _window.clear(sf::Color(0,255,0));
        GameLoop();
        if(_debuger) Debuger::Draw();
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
            _resize = true;
            Game::_scene.view_game.update();
        }
        else if (currentEvent.type == sf::Event::Closed)
        {
            _true_exit=true;
            Game::ExitGame();
        } else if (currentEvent.type == sf::Event::KeyPressed) {
            Inputs::KeyPressed(currentEvent.key.code);
        }
        else if (currentEvent.type == sf::Event::KeyReleased) {
            Inputs::KeyReleased(currentEvent.key.code);
        } else if (currentEvent.type == sf::Event::MouseButtonPressed) {
            Inputs::ButtonPressed(currentEvent.mouseButton.button);
        }
        else if (currentEvent.type == sf::Event::MouseButtonReleased) {
            Inputs::ButtonReleased(currentEvent.mouseButton.button);
        } else if(currentEvent.type == sf::Event::TextEntered && _gameState == Game::NewGame)
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
        }
    }
}
void Game::GameLoop()
{
    sf::Time deltatime = _clock.restart();
    Debuger::Update(deltatime);
    float delta = deltatime.asSeconds();
    Inputs::Update();
    Events();
    if(Inputs::KeyBreak(Inputs::TAB)) {
        Debuger::activated = !Debuger::activated;
        _debuger = !_debuger;
    }
    if(Debuger::activated){
        if(Inputs::KeyBreak(Inputs::F1)) {
            if(Debuger::metric1!="linesChunks")Debuger::metric1 = "linesChunks";
            else Debuger::metric1="none";
        }
        if(Inputs::KeyBreak(Inputs::F2)) {
            if(Debuger::metric2!="tension")Debuger::metric2 = "tension";
            else Debuger::metric2="none";
        }
        if(Inputs::KeyBreak(Inputs::F3)) {
            if(Debuger::metric2!="reachFloor")Debuger::metric2 = "reachFloor";
            else Debuger::metric2="none";
        }
        if(Inputs::KeyBreak(Inputs::F4)) {
            if(Debuger::metric3!="id")Debuger::metric3 = "id";
            else Debuger::metric3="none";
        }
    }
    switch(_gameState)
    {
        case Game::Playing:
        {
            if(Inputs::KeyBreak(Inputs::ESC)) _gameState = Game::ShowingMenu;
            Debuger::InsertClockMark("PreLoop");
            Game::_scene.update(_window,delta);
            Debuger::InsertClockMark("Update");
            Game::_scene.draw(_window);
            Debuger::InsertClockMark("Draw");


            break;
        }
        case Game::ShowingMenu:
        {
            if(_resize){
                MenuMain::view.update();
                _resize = false;
            }
            if(Inputs::KeyBreak(Inputs::ESC) && Game::_scene.map_curr.initialized) _gameState = Game::Playing;
            if(MenuMain::newGameClicked()) _gameState = NewGame;
            else if(MenuMain::exitClicked()) {
                _true_exit=true;
                ExitGame();
            }
            else if(MenuMain::loadClicked()) _gameState = LoadGame;
            else if(MenuMain::configClicked()) _gameState = Config;
            MenuMain::Update();
            MenuMain::Draw(_window, _font);
            break;
        }
        case Game::NewGame:
        {

            if(_resize){
                NewGameMenu::view.update();
                _resize = false;
            }
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
                if(out.size()<100) {


                    std::string new_game_path = "save/s";
                    if (out.size() < 10) new_game_path.append("0");
                    new_game_path.append(std::to_string(out.size()));
                    CreateNewGame(new_game_path, NewGameMenu::seed.getText(), NewGameMenu::name.getText());
                    Game::_scene.init(new_game_path, _window, NewGameMenu::seed.getText());

                    _gameState = Playing;
                } else std::cout << "demasiadas partidas guardadas" << std::endl;
            }


            NewGameMenu::Update();
            NewGameMenu::Draw(_window, _font);
            break;
        }
        case Game::LoadGame:
        {
            if(_resize){
                MenuLoadGame::view.update();
                _resize = false;
            }
            if(MenuLoadGame::backClicked()) _gameState = ShowingMenu;

            else if(MenuLoadGame::loadClicked() && !MenuLoadGame::save_list.elements.empty()) {

                std::string folder_save = "s";
                int slot = MenuLoadGame::save_list.selected_slot;
                if(slot<10) folder_save.append("0");
                if(slot <100){
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

                        Game::_scene.saveGame();
                        Game::_scene.init(new_game_path, _window, data_seed);
                        _gameState = Playing;
                    }
                    else std::cout << "ERROR: could not read file " << route << std::endl;





                } else std::cout << "slot demasiado grande" << std::endl;
            }
            else if(MenuLoadGame::delClicked() && !Game::_scene.map_curr.initialized && !MenuLoadGame::save_list.elements.empty()) {

                std::string folder_save = "s";
                int slot = MenuLoadGame::save_list.selected_slot;
                if(slot<10) folder_save.append("0");
                if(slot <100){
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
            MenuLoadGame::Draw(_window, _font);
            break;

        }

        case Game::Config:
        {
            if(_resize){
                MenuConfigGame::view.update();
                _resize = false;
            }
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
}



void Game::ExitGame()
{
    Game::_scene.saveGame();
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



