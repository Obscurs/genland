//
// Created by arnito on 30/10/16.
//

#include "Drawer.h"
#include "../Game.h"
#include "../Settings.h"

Drawer::Drawer(Map *m,Player *p,WorldBackground *b,Clock *c){
    map_curr = m;
    player = p;
    clock = c;
    backgrounds = b;
    texture_plain_sprite.create(Settings::GAME_WIDTH, Settings::GAME_HEIGHT);
    texture_background.create(Settings::GAME_WIDTH, Settings::GAME_HEIGHT);
    texture_front = new sf::RenderTexture();
    texture_back = new sf::RenderTexture();


    view_player.setSize(Settings::GAME_WIDTH, Settings::GAME_HEIGHT);
    view_player.setSize(Settings::GAME_WIDTH, Settings::GAME_HEIGHT);
    //view_player.zoom(0.5);
    texture_front->create(Settings::GAME_WIDTH, Settings::GAME_HEIGHT);
    texture_back->create(Settings::GAME_WIDTH, Settings::GAME_HEIGHT);
    black_texture.create(Settings::GAME_WIDTH, Settings::GAME_HEIGHT);
    sf::RectangleShape rectangle(sf::Vector2f(0, 0));
    rectangle.setSize(sf::Vector2f(Settings::GAME_WIDTH, Settings::GAME_HEIGHT));
    rectangle.setFillColor(sf::Color::Black);
    black_texture.draw(rectangle);



    // Load the shaders

    if (!sun_background_shader.loadFromFile("resources/sun_background.frag", sf::Shader::Fragment)) std::cout<< "el shader no va" << std::endl;
    if (!tile_shader.loadFromFile("resources/light.frag", sf::Shader::Fragment)) std::cout<< "el shader no va" << std::endl;
    if (!sun_shader.loadFromFile("resources/sun.frag", sf::Shader::Fragment)) std::cout<< "el shader no va" << std::endl;
    if (!sun_mix_shader.loadFromFile("resources/sun_mix.frag", sf::Shader::Fragment)) std::cout<< "el shader no va" << std::endl;
    if (!mix_back_terr_shader.loadFromFile("resources/mix_background_terrain.frag", sf::Shader::Fragment)) std::cout<< "el shader no va" << std::endl;
    sun_background_shader.setParameter("windowHeight", static_cast<float>(Settings::GAME_HEIGHT)); // this must be set, but only needs to be set once (or whenever the size of the window changes)
    tile_shader.setParameter("windowHeight", static_cast<float>(Settings::GAME_HEIGHT)); // this must be set, but only needs to be set once (or whenever the size of the window changes)
    texMan = new TextureManager("resources/tiles2.png", 16, 16);
    texMan->insert_map_value("D",sf::Vector2i(0,0));
    texMan->insert_map_value("d",sf::Vector2i(16,0));
    texMan->insert_map_value("C",sf::Vector2i(0,16));
    texMan->insert_map_value("c",sf::Vector2i(16,16));

    texMan->insert_map_value("D_in",sf::Vector2i(32,0));
    texMan->insert_map_value("d_in",sf::Vector2i(64,0));
    texMan->insert_map_value("C_in",sf::Vector2i(32,16));
    texMan->insert_map_value("c_in",sf::Vector2i(64,16));

    texMan->insert_map_value("D_out",sf::Vector2i(48,0));
    texMan->insert_map_value("d_out",sf::Vector2i(80,0));
    texMan->insert_map_value("C_out",sf::Vector2i(48,16));
    texMan->insert_map_value("c_out",sf::Vector2i(80,16));

    texMan->insert_map_value("r",sf::Vector2i(0,32));
    texMan->insert_map_value("0",sf::Vector2i(64,32));
    texMan->insert_map_value("s",sf::Vector2i(16,32));
    texMan->insert_map_value("S",sf::Vector2i(32,32));
    texMan->insert_map_value("S2",sf::Vector2i(48,32));

    texMan->insert_map_value("grass0",sf::Vector2i(0,48));
    texMan->insert_map_value("grass1",sf::Vector2i(16,48));
}


Drawer::~Drawer()
{

}

void Drawer::DrawFrontItemsMap(sf::RenderWindow& renderWindow,sf::VertexArray &render_array)
{
    sf::View currentView = renderWindow.getView();
    sf::Vector2f centerView = currentView.getCenter();
    sf::Vector2f sizeView = currentView.getSize();
    float first_x = centerView.x-(sizeView.x/2)-1;
    float last_x = centerView.x+(sizeView.x/2)+1;
    int first_chunk = map_curr->getChunkIndex(first_x);
    int last_chunk = map_curr->getChunkIndex(last_x+Settings::TILE_SIZE);
    for(int i = first_chunk ; i<=last_chunk ; ++i) {
        int index_mat = map_curr->getIndexMatChunk(i);
        map_curr->chunk_mat[index_mat]->DrawGrassTiles(*texMan,render_array);
    }
}

sf::Sprite Drawer::get_plain_sprite(sf::RenderWindow& renderWindow,sf::VertexArray &render_array,sf::VertexArray &sky_array){
    texture_plain_sprite.clear(sf::Color(0,0,0,0));
    //backgrounds->Draw(texture_plain_sprite);

    sf::Vector2f centerView = view_player.getCenter();
    sf::Vector2f sizeView = view_player.getSize();
    float first_x = centerView.x-(sizeView.x/2)-1;
    float first_y = centerView.y-(sizeView.y/2)-1;
    float last_x = centerView.x+(sizeView.x/2)+1;
    float last_y = centerView.y+(sizeView.y/2)+1;
    //std::cout << first_x << " " << first_y << std::endl;
    sf::Vector2f firstPos(first_x, first_y);
    sf::Vector2f lastPos(last_x+Settings::TILE_SIZE, last_y+Settings::TILE_SIZE);
    int first_chunk = map_curr->getChunkIndex(first_x);
    int last_chunk = map_curr->getChunkIndex(last_x+Settings::TILE_SIZE);

    if(clock->min<20){
        sun_background_shader.setParameter("color", sf::Color::Black);
        sun_background_shader.setParameter("color2", sf::Color::Black);
        sun_background_shader.setParameter("factor2", 0);
    } else if(clock->min<30){
        sun_background_shader.setParameter("color", sf::Color::Black);
        sun_background_shader.setParameter("color2", sf::Color(244, 173, 66));
        sun_background_shader.setParameter("factor2", (clock->min-20)/10);
    }else if(clock->min<40){
        sun_background_shader.setParameter("color", sf::Color(244, 173, 66));
        sun_background_shader.setParameter("color2", sf::Color::Yellow);
        sun_background_shader.setParameter("factor2", (clock->min-30)/10);
    }
    else if(clock->min<50){
        sun_background_shader.setParameter("color", sf::Color::Yellow);
        sun_background_shader.setParameter("color2", sf::Color::Blue);
        sun_background_shader.setParameter("factor2", (clock->min-40)/10);
    }
    else if(clock->min<60){
        sun_background_shader.setParameter("color", sf::Color::Blue);
        sun_background_shader.setParameter("color2", sf::Color::Black);
        sun_background_shader.setParameter("factor2", (clock->min-50)/10);
    }
    if(clock->min<20)sun_background_shader.setParameter("factor", 1.0);
    else if(clock->min<30) sun_background_shader.setParameter("factor", 1.0-(clock->min-20)/10);
    else if(clock->min<50) sun_background_shader.setParameter("factor", 0.0);
    else sun_background_shader.setParameter("factor", (clock->min-50)/10);

    texture_background.clear(sf::Color(255,0,0,255));

    backgrounds->Draw(texture_background);
    sf::Vector2f pos_sprite = firstPos;
    pos_sprite.x+=1;
    pos_sprite.y+=1;
    //std::cout <<pos_sprite.x << std::endl;
    sf::Sprite background_sprite(texture_background.getTexture());
    //background_sprite.setScale(sf::Vector2f(0.5,0.5));
    background_sprite.setPosition(pos_sprite);
    //return background_sprite;

    sf::RenderStates states2;
    texture_background.display();
    states2.texture = &texture_background.getTexture();
    states2.shader = &sun_background_shader;
    texture_background.draw(background_sprite, states2);
    texture_background.display();

    for(int i = first_chunk ; i<=last_chunk ; ++i) {
        int index_mat = map_curr->getIndexMatChunk(i);
        map_curr->chunk_mat[index_mat]->DrawChunk(firstPos, lastPos, *texMan, tile_shader,render_array, sky_array);
    }

    for(int i = 0; i<map_curr->falling_tiles.size(); i++){

        map_curr->falling_tiles[i]->Draw(renderWindow, *texMan);
    }
    DrawFrontItemsMap(renderWindow,render_array);

    sf::RenderStates states;
    states.texture = texMan->getTexture();
    texture_plain_sprite.draw(render_array, states);

    texture_plain_sprite.display();
    sf::Sprite map_without_lights(texture_plain_sprite.getTexture());
    //map_without_lights.setScale(sf::Vector2f(0.5,0.5));
    map_without_lights.setPosition(pos_sprite);

    return map_without_lights;
}

void Drawer::DrawLights(sf::VertexArray &render_array,sf::VertexArray &sky_array, sf::Sprite map_without_lights){
    sf::RenderStates states;
    states.texture = texMan->getTexture();

    //DRAWING SUN
    states.shader = &sun_shader;
    texture_back->clear(sf::Color(0,0,0,0));
    texture_back->draw(sky_array, states);
    texture_back->display();

    if(clock->min<20){
        sun_mix_shader.setParameter("color", sf::Color::Black);
        sun_mix_shader.setParameter("color2", sf::Color::Black);
        sun_mix_shader.setParameter("factor2", 0);
    } else if(clock->min<30){
        sun_mix_shader.setParameter("color", sf::Color::Black);
        sun_mix_shader.setParameter("color2", sf::Color(244, 173, 66));
        sun_mix_shader.setParameter("factor2", (clock->min-20)/10);
    }else if(clock->min<40){
        sun_mix_shader.setParameter("color", sf::Color(244, 173, 66));
        sun_mix_shader.setParameter("color2", sf::Color::Yellow);
        sun_mix_shader.setParameter("factor2", (clock->min-30)/10);
    }
    else if(clock->min<50){
        sun_mix_shader.setParameter("color", sf::Color::Yellow);
        sun_mix_shader.setParameter("color2", sf::Color::Blue);
        sun_mix_shader.setParameter("factor2", (clock->min-40)/10);
    }
    else if(clock->min<60){
        sun_mix_shader.setParameter("color", sf::Color::Blue);
        sun_mix_shader.setParameter("color2", sf::Color::Black);
        sun_mix_shader.setParameter("factor2", (clock->min-50)/10);
    }
    if(clock->min<20)sun_mix_shader.setParameter("factor", 1.0);
    else if(clock->min<30) sun_mix_shader.setParameter("factor", 1.0-(clock->min-20)/10);
    else if(clock->min<50) sun_mix_shader.setParameter("factor", 0.0);
    else sun_mix_shader.setParameter("factor", (clock->min-50)/10);


    sun_mix_shader.setParameter("texture2", texture_back->getTexture());
    states.shader = &sun_mix_shader;
    texture_front->clear(sf::Color(0,0,0,0));

    texture_front->draw(map_without_lights, states);
    texture_front->display();

    std::swap(texture_back,texture_front);

    //END DRAWING SUN
    //DRAWING MAP LIGHTS
    //texture_back->setView(currentView);
    //texture_front->setView(currentView);
    sf::Vector2f centerView = view_player.getCenter();
    sf::Vector2f sizeView = view_player.getSize();
    float first_x = centerView.x-(sizeView.x/2)-1;
    float first_y = centerView.y-(sizeView.y/2)-1;

    for(int i = 0; i<map_curr->lights.size(); i++){
    //for(int i = 0; i<4; i++){
        float radius = map_curr->lights[i].radius;
        sf::Vector2f lightpos = sf::Vector2f(map_curr->lights[i].position.x-first_x,map_curr->lights[i].position.y-first_y);
        if(lightpos.x>0-radius*2 && lightpos.x<sizeView.x+radius*2 && lightpos.y>0-radius*2 && lightpos.y<sizeView.y+radius*2) {

            map_curr->lights[i].Draw(lightpos, map_without_lights, tile_shader, texMan, texture_front, texture_back);
            std::swap(texture_back, texture_front);
        }
    }

}

void Drawer::DrawMap(sf::RenderWindow& renderWindow,float zoom)
{
    //texture_back->setView(mv);
    //texture_front->setView(mv);
    //texture_plain_sprite.setView(mv);
    //black_texture.setView(mv);
    //texture_background.setView(mv);

    sf::VertexArray render_array(sf::Quads , (uint)(4));
    sf::VertexArray sky_array(sf::Quads , (uint)(4));
    sf::Sprite map_without_lights = get_plain_sprite(renderWindow, render_array,sky_array);
    //view_player.setSize(sf::Vector2f(1000,600));
    //std::cout << map_without_lights.getPosition().x << " " << " "<< std::endl;
    DrawLights(render_array,sky_array,map_without_lights);
    texture_back->display();
    player->Draw2(*texture_back);
    sf::Sprite sprite(texture_back->getTexture());



    sprite.setPosition(map_without_lights.getPosition());
    sf::Vector2f oldOrgin = sprite.getOrigin();
    sprite.setOrigin(sprite.getTexture()->getSize().x/(2),sprite.getTexture()->getSize().y/(2));

    sprite.setScale(sf::Vector2f(zoom,zoom));
    sprite.setPosition(sprite.getPosition().x+sprite.getTexture()->getSize().x/(2), sprite.getPosition().y+sprite.getTexture()->getSize().y/(2));

    mix_back_terr_shader.setParameter("texture2", texture_background.getTexture());
    sf::RenderStates states;
    states.texture = &texture_back->getTexture();
    states.shader = &mix_back_terr_shader;

    renderWindow.draw(sprite, states);
    //view_player.setSize(Settings::GAME_WIDTH, Settings::GAME_HEIGHT);
}

void Drawer::Draw(sf::RenderWindow &window, float zoom){


    view_player.setCenter(player->GetPosition().x+(player->GetWidth()/2), player->GetPosition().y+(player->GetHeight()/2));

    texture_back->setView(view_player);
    texture_front->setView(view_player);
    texture_plain_sprite.setView(view_player);
    texture_background.setView(view_player);
    black_texture.setView(view_player);

    DrawMap(window, zoom);

    player->DrawInventory(window);

}
