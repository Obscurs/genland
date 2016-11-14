#include <stdio.h>
#include <math.h>
// TODO: reference additional headers your program requires here
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <cassert>


#include <string.h>
#include <limits.h>
#include <unistd.h>

#include "Game.h"


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


void Game::Start(void)
{

    if(_gameState != Uninitialized)
        return;

    window.create(sf::VideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,32),"Genland!");

    sf::View viewPlayer(sf::FloatRect(200, 200, SCREEN_WIDTH, SCREEN_HEIGHT));
    view_game.setRenderTarget(&window);
    view_game.setViewport({0,0,1,1});
    view_game.setResolution(sf::Vector2i(SCREEN_WIDTH,SCREEN_HEIGHT));
    view_game.setMode(MagicView::crop);
    window.setView(view_game);

    _gameState= Game::Playing;
    sf::Clock clock1;
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

        window.clear(sf::Color(0,255,0,0));
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
    view_game.update();
    switch(_gameState)
    {
        case Game::ShowingMenu:
        {

            MenuMain::Draw(window, font);
            MenuMain::Update(); //s'ha de fer despres de draw pk sino peta magicView
            if(MenuMain::newGameClicked(Game::inputs)) _gameState = NewGame;
            else if(MenuMain::exitClicked(Game::inputs)) ExitGame();
            else if(MenuMain::loadClicked(Game::inputs)) _gameState = LoadGame;
            while(window.pollEvent(currentEvent))
            {
                if(currentEvent.type == sf::Event::MouseWheelMoved)
                {
                    Game::inputs.UpdateWheel(currentEvent.mouseWheel.delta);
                }

                else if (
                         (currentEvent.type == sf::Event::KeyPressed) &&
                          (currentEvent.key.code == sf::Keyboard::Escape))
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
            break;
        }
        case Game::NewGame:
        {
            NewGameMenu::Draw(window, font);
            NewGameMenu::Update();
            if(NewGameMenu::backClicked(Game::inputs)) _gameState = ShowingMenu;
            else if(NewGameMenu::startClicked(Game::inputs)) {
                std::vector<std::string> out;
                GetFilesInDirectory(out,"save");
                std::string new_game_path = "save/s";
                new_game_path.append(std::to_string(out.size()));
                CreateNewGame(new_game_path);
                Game::game.restart(new_game_path,window);

                _gameState = Playing;
                //DeleteGame(2,"save");
                //int a = DeleteDirectory("save/s1");
                //std::cout << out[0] << std::endl;
            }
            while(window.pollEvent(currentEvent))
            {
                if(currentEvent.type == sf::Event::MouseWheelMoved)
                {
                    Game::inputs.UpdateWheel(currentEvent.mouseWheel.delta);
                }
                else if (currentEvent.type == sf::Event::Resized){
                    NewGameMenu::view.update();
                    std::cout << "res" << std::endl;
                }
                else if (currentEvent.type == sf::Event::Closed)
                {
                    Game::ExitGame();
                }
            }
            break;
        }
        case Game::LoadGame:
        {
            std::vector<std::string> out;
            GetFilesInDirectory(out,"save");
            std::string new_game_path = "save/s";
            new_game_path.append(std::to_string(out.size()-1));
            Game::game.restart(new_game_path,window);
            _gameState = Playing;


            break;
        }
        case Game::Playing:
        {

            Game::game.update(window,view_game,delta,inputs);

            Game::game.draw(window);


            while(window.pollEvent(currentEvent))
	        {
                if(currentEvent.type == sf::Event::MouseWheelMoved)
		        {
                    Game::inputs.UpdateWheel(currentEvent.mouseWheel.delta);
                }
                else if (currentEvent.type == sf::Event::Resized){
                    std::cout << "res" << std::endl;
                }
                else if (
                        (currentEvent.type == sf::Event::KeyPressed) &&
                        (currentEvent.key.code == sf::Keyboard::Escape))
                {
                    std::cout << "bye" << std::endl;
                    _gameState = ShowingMenu;
                    //window.close();
                }
                else if (currentEvent.type == sf::Event::Closed)
                {
                    Game::ExitGame();
                }
            }
            break;
        }
    }
}



void Game::ExitGame()
{
    Game::game.saveGame();
    _gameState = Exiting;
}
void Game::CreateNewGame(std::string path) {
    const int dir_err = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == dir_err)
    {
        printf("Error creating directory s !n");
        exit(1);
    }
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
    path_delete.append(std::to_string(index));
    DeleteDirectory(path_delete.c_str());
    int result;
    for(int i=index+1; i<out.size();i++){
        std::string path_rename_old = path;
        path_rename_old.append("/s");
        std::string path_rename_new = path_rename_old;
        path_rename_old.append(std::to_string(i));
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
} // GetFilesInDirectory


Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::window;

Inputs Game::inputs;
RunningGame Game::game("save/s0",window);
MagicView Game::view_game;