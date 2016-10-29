//
// Created by ant on 17/10/16.
//

#include <iostream>
#include "Light.h"

#include <random>
Light::Light(sf::Vector2f pos, float rad_min, float rad_max, float exp, sf::Color c) {
    position=pos;
    radius_max = rad_max;
    radius_min = rad_min;
    expand = exp;
    color = c;
    radius = radius_min;
}

Light::~Light()
{
    //std::for_each(_gameObjects.begin(),_gameObjects.end(),GameObjectDeallocator());
}
void Light::Update(float deltatime){
    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni((int)radius_min,(int)radius_max); // guaranteed unbiased

    auto random_integer = uni(rng);
    radius = random_integer;
    /*
    int direction = rand()%(1-0 + 1);
    if(direction==0 && radius>radius_min)radius -= deltatime*10;
    else if(direction==0)radius += deltatime;
    else if(direction==1 && radius<radius_max)radius += deltatime*10;
    else if(direction==1)radius -= deltatime;
     */
}

void Light::Draw(sf::View& currentView, sf::Sprite &map_without_lights, sf::Shader &light_shader, TextureManager *t, sf::RenderTexture *tex_front, sf::RenderTexture *tex_back){

    sf::Vector2f centerView = currentView.getCenter();
    sf::Vector2f sizeView = currentView.getSize();
    float first_x = centerView.x-(sizeView.x/2)-1;
    float first_y = centerView.y-(sizeView.y/2)-1;

    sf::Vector2f lightpos = sf::Vector2f(position.x-first_x,position.y-first_y);
    if(lightpos.x>0-radius*2 && lightpos.x<sizeView.x+radius*2 && lightpos.y>0-radius*2 && lightpos.y<sizeView.y+radius*2) {


        sf::RenderStates states;
        states.texture = t->getTexture();

        light_shader.setParameter("texture2", tex_back->getTexture());
        light_shader.setParameter("color", color);
        light_shader.setParameter("center", lightpos);
        light_shader.setParameter("radius", radius);
        light_shader.setParameter("expand", expand);

        states.shader = &light_shader;
        tex_front->clear(sf::Color::Blue);
        tex_front->setView(currentView);
        tex_front->draw(map_without_lights, states);
        tex_front->display();


    }
}