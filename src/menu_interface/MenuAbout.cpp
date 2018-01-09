//
// Created by arnito on 8/01/18.
//

#include "MenuAbout.h"

#include "../Game.h"
#include "../Inputs.h"
#include "../Resources.h"

void MenuAbout::Draw(sf::RenderWindow &window, sf::Font font)
{
    const sf::View* oldView = &window.getView();
    window.setView(view);
    sf::Sprite sprite(*Resources::getTexture("about"));
    back.Draw(window, font);
    window.draw(sprite);
    window.setView(*oldView);
}
void MenuAbout::Update()
{
    sf::Vector2f mousePos = view.getMouseCoord();
    back.update(mousePos);

}



bool MenuAbout::backClicked(){
    return (Inputs::MouseBreak(Inputs::M_LEFT) && back.mouseOver);
}
Button MenuAbout::back(sf::Vector2f(10,1800),
                      sf::Vector2f(300,90),
                      "BACK",
                      0);
MagicView MenuAbout::view;