#include <stdio.h>
#include <math.h>
// TODO: reference additional headers your program requires here
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include <dirent.h>
#include <fstream>
#include <sys/stat.h>
#include <string.h>

#include <limits.h>
#include <unistd.h>
#include <unistd.h>
#include <cassert>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "Game.h"
#include "Settings.h"
//bool sortByName(const Person &lhs, const Person &rhs) { return lhs.name < rhs.name; }

inline bool exists_file (const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}
int DeleteDirectory(const char *dirname)
{
    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];

    if (path == NULL) {
        fprintf(stderr, "Out of memory error\n");
        return 0;
    }
    dir = opendir(dirname);
    if (dir == NULL) {
        perror("Error opendir()");
        return 0;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            snprintf(path, (size_t) PATH_MAX, "%s/%s", dirname, entry->d_name);
            if (entry->d_type == DT_DIR) {
                DeleteDirectory(path);
            }

            printf("(not really) Deleting: %s\n", path);
            remove(path);

        }

    }
    closedir(dir);
    remove(dirname);
    printf("(not really) Deleting: %s\n", dirname);

    return 1;
}

Game::Game():
        inputs(),
        window(),
        game(window)
{
    _gameState = Uninitialized;
    true_exit=false;

}
Game::~Game(){

}
void Game::GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory)
{
#ifdef WINDOWS
    HANDLE dir;
    WIN32_FIND_DATA file_data;

    if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
        return; /* No files found */

    do {
        const string file_name = file_data.cFileName;
        const string full_file_name = directory + "/" + file_name;
        const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

        if (file_name[0] == '.')
            continue;

        if (is_directory)
            continue;

        out.push_back(full_file_name);
    } while (FindNextFile(dir, &file_data));

    FindClose(dir);
#else
    DIR *dir;
    class dirent *ent;
    class stat st;
    const char * c = directory.c_str();
    dir = opendir(c);
    while ((ent = readdir(dir)) != NULL) {

        const std::string file_name = ent->d_name;
        const std::string full_file_name = directory + "/" + file_name;

        if (file_name[0] == '.')
            continue;

        if (stat(full_file_name.c_str(), &st) == -1)
            continue;

        //const bool is_directory = (st.st_mode & S_IFDIR) != 0;

        //if (is_directory)
        //    continue;
        out.push_back(full_file_name);
    }
    closedir(dir);
#endif
    std::sort (out.begin(), out.end());
} // GetFilesInDirectory



void Game::LoadData(){

    std::vector<std::string> save_folders;
    GetFilesInDirectory(save_folders, "save");
    MenuLoadGame::save_list.elements.clear();
    for(int i = 0; i<save_folders.size(); i++){
        std::string route = save_folders[i];
        route.append("/data");
        if(exists_file(route)) {
            //std::string route_aux = route;
            //route_aux.append(".txt");
            std::ifstream myfile(route);
            //myfile.open(route);
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

    if(_gameState != Uninitialized)
        return;
    LoadData();
    window.create(sf::VideoMode(Settings::SCREEN_WIDTH,Settings::SCREEN_HEIGHT,32),"Genland!");

    // sf::View viewPlayer(sf::FloatRect(200, 200, Settings::SCREEN_WIDTH, Settings::SCREEN_HEIGHT));

    _gameState= Game::ShowingMenu;
    sf::Clock clock1;
    sf::Clock clock2;
    float lastTime = 0;
//hola
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

    MenuMain::view = MagicView(&window,MagicView::expanded,sf::Vector2i(2000,2000));
    NewGameMenu::view = MagicView(&window,MagicView::expanded,sf::Vector2i(2000,2000));
    MenuLoadGame::view = MagicView(&window,MagicView::expanded,sf::Vector2i(2000,2000));
    MenuConfigGame::view = MagicView(&window,MagicView::expanded,sf::Vector2i(2000,2000));

    while(!IsExiting())
    {


        double delta =  clock1.restart().asSeconds();


        float currentTime = clock2.restart().asSeconds();
        float fps = 1.f / currentTime;
        fps_count += 1;
        fps_count2 += fps;
        //fps = roundf(fps/10)*10;
        lastTime = currentTime;
        //std::cout << fps << std::endl;

        window.clear(sf::Color(0,255,0));
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
    sf::Font font;
    if (!font.loadFromFile("resources/font1.ttf"))
    {
        std::cout << "font error" << std::endl;
    }
    Game::inputs.Update();
    switch(_gameState)
    {
        case Game::ShowingMenu:
        {
            if(MenuMain::newGameClicked(Game::inputs)) _gameState = NewGame;
            else if(MenuMain::exitClicked(Game::inputs)) {
                true_exit=true;
                ExitGame();
            }
            else if(MenuMain::loadClicked(Game::inputs)) _gameState = LoadGame;
            else if(MenuMain::configClicked(Game::inputs)) _gameState = Config;
            while(window.pollEvent(currentEvent))
            {
                if(currentEvent.type == sf::Event::MouseWheelMoved)
                {
                    Game::inputs.UpdateWheel(currentEvent.mouseWheel.delta);
                }

                else if (
                         (currentEvent.type == sf::Event::KeyPressed) &&
                          (currentEvent.key.code == sf::Keyboard::Tab) &&
                         Game::game.map_curr.initialized)
                {
                    std::cout << "bye" << std::endl;
                    _gameState = Playing;
                    //window.close();
                }
                else if (currentEvent.type == sf::Event::Resized){
                    MenuMain::view.update();
                    std::cout << "res" << std::endl;
                }
                else if (currentEvent.type == sf::Event::Closed)
                {
                    Game::ExitGame();
                }
            }
            MenuMain::Update();
            MenuMain::Draw(window, font);
            break;
        }
        case Game::NewGame:
        {


            if(NewGameMenu::backClicked(Game::inputs)) _gameState = ShowingMenu;
            else if(NewGameMenu::seedClicked(Game::inputs)){
                NewGameMenu::disSelectAll();
                NewGameMenu::seed.selected = true;
            }
            else if(NewGameMenu::nameClicked(Game::inputs)){
                NewGameMenu::disSelectAll();
                NewGameMenu::name.selected = true;
            }
            else if(NewGameMenu::startClicked(Game::inputs) && NewGameMenu::seed.text != "") {
                std::vector<std::string> out;
                GetFilesInDirectory(out,"save");
                if(out.size()<100) {


                    std::string new_game_path = "save/s";
                    if (out.size() < 10) new_game_path.append("0");
                    new_game_path.append(std::to_string(out.size()));
                    CreateNewGame(new_game_path, NewGameMenu::seed.getText(), NewGameMenu::name.getText());
                    Game::game.restart(new_game_path, window, NewGameMenu::seed.getText());

                    _gameState = Playing;
                } else std::cout << "demasiadas partidas guardadas" << std::endl;
            }
            while(window.pollEvent(currentEvent))
            {
                if(currentEvent.type == sf::Event::TextEntered)
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
                else if (currentEvent.type == sf::Event::Resized){
                    NewGameMenu::view.update();
                    std::cout << "res" << std::endl;
                }
                else if (currentEvent.type == sf::Event::Closed)
                {
                    true_exit=true;
                    Game::ExitGame();
                }
            }
            NewGameMenu::Update();
            NewGameMenu::Draw(window, font);
            break;
        }
        case Game::LoadGame:
        {
            if(MenuLoadGame::backClicked(Game::inputs)) _gameState = ShowingMenu;

            else if(MenuLoadGame::loadClicked(Game::inputs) && !MenuLoadGame::save_list.elements.empty()) {

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

                        Game::game.saveGame();
                        Game::game.restart(new_game_path,window, data_seed);
                        _gameState = Playing;
                    }
                    else std::cout << "ERROR: could not read file " << route << std::endl;





                } else std::cout << "slot demasiado grande" << std::endl;
            }
            else if(MenuLoadGame::delClicked(Game::inputs) && !Game::game.map_curr.initialized && !MenuLoadGame::save_list.elements.empty()) {

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
            while(window.pollEvent(currentEvent))
            {
                if(currentEvent.type == sf::Event::MouseWheelMoved)
                {
                    Game::inputs.UpdateWheel(currentEvent.mouseWheel.delta);
                }
                else if (currentEvent.type == sf::Event::Resized){
                    MenuLoadGame::view.update();
                    std::cout << "res" << std::endl;
                }
                else if (currentEvent.type == sf::Event::Closed)
                {
                    true_exit=true;
                    Game::ExitGame();
                }
            }
            MenuLoadGame::Update(Game::inputs);
            MenuLoadGame::Draw(window, font);
            break;

        }
        case Game::Playing:
        {
            while(window.pollEvent(currentEvent))
	        {
                if(currentEvent.type == sf::Event::MouseWheelMoved)
		        {
                    Game::inputs.UpdateWheel(currentEvent.mouseWheel.delta);
                }
                else if (currentEvent.type == sf::Event::Resized){
                    std::cout << "res" << std::endl;
                    Game::game.view_game.update();
                }
                else if (
                        (currentEvent.type == sf::Event::KeyPressed) &&
                        (currentEvent.key.code == sf::Keyboard::Tab))
                {
                    std::cout << "bye" << std::endl;
                    _gameState = ShowingMenu;
                    //window.close();
                }
                else if (currentEvent.type == sf::Event::Closed)
                {
                    true_exit=true;
                    Game::ExitGame();
                }
            }
            Game::game.update(window,delta,inputs);
            Game::game.draw(window);
            break;
        }
        case Game::Config:
        {
            if(MenuConfigGame::backClicked(Game::inputs) && !MenuConfigGame::resolution_visible) _gameState = ShowingMenu;
            else if(MenuConfigGame::resolution_visible && Game::inputs.getKey("Enter").y){
                MenuConfigGame::resolution_visible = false;
                int x = MenuConfigGame::res_keys[MenuConfigGame::resolution_list.selected_slot*2];
                int y = MenuConfigGame::res_keys[MenuConfigGame::resolution_list.selected_slot*2+1];
                window.setSize(sf::Vector2u(x,y));
                MenuConfigGame::view.update();

                if(exists_file("config/config")) {
                    std::ofstream myfile;
                    myfile.open("config/config");
                    myfile << x;
                    myfile << "\n";
                    myfile << y;
                    myfile.close();
                }
                true_exit=false;
                ExitGame();
            }
            else if(MenuConfigGame::resClicked(Game::inputs)) {
                MenuConfigGame::resolution_visible = true;


            }

            while(window.pollEvent(currentEvent))
            {
                if(currentEvent.type == sf::Event::MouseWheelMoved)
                {
                    Game::inputs.UpdateWheel(currentEvent.mouseWheel.delta);
                }
                else if (currentEvent.type == sf::Event::Resized){
                    MenuConfigGame::view.update();
                    std::cout << "res" << std::endl;
                }
                else if (currentEvent.type == sf::Event::Closed)
                {
                    true_exit=true;
                    Game::ExitGame();
                }
            }
            MenuConfigGame::Update(Game::inputs);
            MenuConfigGame::Draw(window, font);
            break;
        }
    }
}



void Game::ExitGame()
{
    Game::game.saveGame();
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



