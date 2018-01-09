//
// Created by arnito on 8/01/18.
//

#include "MenuHelp.h"

#include "../Game.h"
#include "../Inputs.h"
#include "../Resources.h"

void MenuHelp::Draw(sf::RenderWindow &window, sf::Font font)
{
    const sf::View* oldView = &window.getView();
    window.setView(view);
    std::string h = "help";
    h.append(std::to_string(currentHelp));
    sf::Sprite sprite(*Resources::getTexture(h));
    back.Draw(window, font);
    next.Draw(window, font);
    window.draw(sprite);
    window.setView(*oldView);
}
void MenuHelp::Update()
{
    sf::Vector2f mousePos = view.getMouseCoord();
    back.update(mousePos);
    next.update(mousePos);

}

void MenuHelp::nextImage() {
    currentHelp += 1;
    if(currentHelp>7) currentHelp = 1;
}

bool MenuHelp::backClicked(){
    return (Inputs::MouseBreak(Inputs::M_LEFT) && back.mouseOver);
}
bool MenuHelp::nextClicked(){
    return (Inputs::MouseBreak(Inputs::M_LEFT) && next.mouseOver);
}
Button MenuHelp::back(sf::Vector2f(10,1800),
                          sf::Vector2f(300,90),
                          "BACK",
                          0);
Button MenuHelp::next(sf::Vector2f(1700,1800),
                          sf::Vector2f(300,90),
                          "NEXT",
                          0);
MagicView MenuHelp::view;
int MenuHelp::currentHelp = 1;