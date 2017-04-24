//
// Created by ant on 06/11/16.
//

#include "MenuConfigGame.h"

void MenuConfigGame::Draw(sf::RenderWindow &window, sf::Font font)
{
    const sf::View* oldView = &window.getView();
    window.setView(view);
    back.Draw(window, font);
    res.Draw(window, font);
    if(resolution_visible) resolution_list.Draw(window, font);
    window.setView(*oldView);
}
void MenuConfigGame::Update()
{
    sf::Vector2f mousePos = view.getMouseCoord();
    back.update(mousePos);
    if(resolution_visible) resolution_list.update(mousePos);
    else {
        back.update(mousePos);
        res.update(mousePos);
    }

}

bool MenuConfigGame::resClicked(){
    return (Inputs::MouseBreak(Inputs::M_LEFT) && res.mouseOver);
}
bool MenuConfigGame::backClicked(){
    return (Inputs::MouseBreak(Inputs::M_LEFT) && back.mouseOver);
}
Button MenuConfigGame::back(sf::Vector2f(1700,1900),
                          sf::Vector2f(300,90),
                          "BACK",
                          0);
Button MenuConfigGame::res(sf::Vector2f(850,1000),
                            sf::Vector2f(300,90),
                            "RESOLUTION",
                            0);
bool MenuConfigGame::resolution_visible = false;
std::vector<int> MenuConfigGame::res_keys = {1920, 1080, 1600, 900, 1360, 768, 1024, 768, 800, 600};
std::vector<std::string> s = {"1920 x 1080", "1600 x 900","1360 x 768", "1024 x 768", "800 x 600"};
InterfaceList MenuConfigGame::resolution_list(sf::Vector2f(850,500),
                                      sf::Vector2f(300,90),
                                      0,
                                      5,  s);
MagicView MenuConfigGame::view;