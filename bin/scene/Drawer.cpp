//
// Created by arnito on 30/10/16.
//

#include "Drawer.h"
#include "../Game.h"
#include "../Settings.h"

Drawer::Drawer(Map *m,Player *p,WorldBackground *b,Clock *c)
        : texMan("resources/tiles2.png", 16, 16)
{
    map_curr = m;
    player = p;
    clock = c;
    backgrounds = b;
    texture_scene.create(Settings::GAME_WIDTH, Settings::GAME_HEIGHT);
    texture_background.create(Settings::GAME_WIDTH, Settings::GAME_HEIGHT);
    texture_sun.create(Settings::GAME_WIDTH, Settings::GAME_HEIGHT);
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

    texMan.insert_map_value("D",sf::Vector2i(0,0));
    texMan.insert_map_value("d",sf::Vector2i(16,0));
    texMan.insert_map_value("C",sf::Vector2i(0,16));
    texMan.insert_map_value("c",sf::Vector2i(16,16));

    texMan.insert_map_value("D_in",sf::Vector2i(32,0));
    texMan.insert_map_value("d_in",sf::Vector2i(64,0));
    texMan.insert_map_value("C_in",sf::Vector2i(32,16));
    texMan.insert_map_value("c_in",sf::Vector2i(64,16));

    texMan.insert_map_value("D_out",sf::Vector2i(48,0));
    texMan.insert_map_value("d_out",sf::Vector2i(80,0));
    texMan.insert_map_value("C_out",sf::Vector2i(48,16));
    texMan.insert_map_value("c_out",sf::Vector2i(80,16));

    texMan.insert_map_value("r",sf::Vector2i(0,32));
    texMan.insert_map_value("0",sf::Vector2i(64,32));
    texMan.insert_map_value("s",sf::Vector2i(16,32));
    texMan.insert_map_value("S",sf::Vector2i(32,32));
    texMan.insert_map_value("S2",sf::Vector2i(48,32));

    texMan.insert_map_value("grass0",sf::Vector2i(0,48));
    texMan.insert_map_value("grass1",sf::Vector2i(16,48));


}


Drawer::~Drawer()
{

}
TextureManager& Drawer::getTextureManager(){
    return texMan;
}
sf::Shader& Drawer::getTileShader(){
    return tile_shader;
}

sf::Vector2f Drawer::GetPosSprite(){
    sf::Vector2f centerView = view_player.getCenter();
    sf::Vector2f sizeView = view_player.getSize();
    float first_x = centerView.x-(sizeView.x/2)-1;
    float first_y = centerView.y-(sizeView.y/2)-1;
    sf::Vector2f firstPos(first_x, first_y);
    firstPos.x+=1;
    firstPos.y+=1;
    return firstPos;
}
void Drawer::DrawSceneTex(){

    texture_scene.clear(sf::Color(0,0,0,0));
    sf::RenderStates states;
    states.texture = texMan.getTexture();

    sf::Vector2f centerView = view_player.getCenter();
    sf::Vector2f sizeView = view_player.getSize();
    float first_x = centerView.x-(sizeView.x/2)-1;
    float last_x = centerView.x+(sizeView.x/2)+1;
    int first_chunk = map_curr->getChunkIndex(first_x);
    int last_chunk = map_curr->getChunkIndex(last_x+Settings::TILE_SIZE);
    for(int i = first_chunk ; i<=last_chunk ; i = i +1) {
        int index_mat = map_curr->getIndexMatChunk(i);
        texture_scene.draw(map_curr->chunk_mat[index_mat]->render_array, states);
    }
    for(int i = 0; i<map_curr->falling_tiles.size(); i++){
        //map_curr->falling_tiles[i]->Draw(renderWindow, texMan);
    }
    texture_scene.display();
}
void Drawer::DrawBackground() {
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

    sf::Sprite background_sprite(texture_background.getTexture());
    sf::Vector2f pos_sprite = GetPosSprite();
    background_sprite.setPosition(pos_sprite);

    sf::RenderStates states2;
    texture_background.display();
    states2.texture = &texture_background.getTexture();
    states2.shader = &sun_background_shader;
    texture_background.draw(background_sprite, states2);
    texture_background.display();
}
void Drawer::DrawLights(){

    sf::Sprite map_without_lights(texture_scene.getTexture());
    sf::Vector2f pos_sprite = GetPosSprite();
    map_without_lights.setPosition(pos_sprite);
    sf::RenderStates states;
    states.texture = texMan.getTexture();

    //DRAWING SUN
    states.shader = &sun_shader;
    texture_sun.clear(sf::Color(0,0,0,0));
    sf::Vector2f centerView = view_player.getCenter();
    sf::Vector2f sizeView = view_player.getSize();
    float first_x = centerView.x-(sizeView.x/2)-1;
    float first_y = centerView.y-(sizeView.y/2)-1;
    float last_x = centerView.x+(sizeView.x/2)+1;
    float last_y = centerView.y+(sizeView.y/2)+1;
    int first_chunk = map_curr->getChunkIndex(first_x);
    int last_chunk = map_curr->getChunkIndex(last_x+Settings::TILE_SIZE);

    for(int i = first_chunk ; i<=last_chunk ; i++) {
        int index_mat = map_curr->getIndexMatChunk(i);
        texture_sun.draw(map_curr->chunk_mat[index_mat]->sky_array, states);

    }
    texture_sun.display();

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


    sun_mix_shader.setParameter("texture2", texture_sun.getTexture());
    states.shader = &sun_mix_shader;
    texture_back->clear(sf::Color(0,0,0,0));
    texture_front->clear(sf::Color(0,0,0,0));
    texture_front->draw(map_without_lights, states);
    texture_front->display();

    std::swap(texture_back,texture_front);

    //END DRAWING SUN
    //DRAWING MAP LIGHTS
    //texture_back->setView(currentView);
    //texture_front->setView(currentView);


    for(int i = 0; i<map_curr->lights.size(); i++){
        //for(int i = 0; i<4; i++){
        float radius = map_curr->lights[i].radius;
        sf::Vector2f lightpos = sf::Vector2f(map_curr->lights[i].position.x-first_x,map_curr->lights[i].position.y-first_y);
        if(lightpos.x>0-radius*2 && lightpos.x<sizeView.x+radius*2 && lightpos.y>0-radius*2 && lightpos.y<sizeView.y+radius*2) {

            map_curr->lights[i].Draw(lightpos, map_without_lights, tile_shader, &texMan, texture_front, texture_back,texture_sun);
            std::swap(texture_back, texture_front);
        }
    }

}

void Drawer::DrawMap(sf::RenderWindow& renderWindow,float zoom)
{
    //sf::Clock clock1;
    //clock1.restart().asSeconds();


    //sf::Time elapsed1 =clock1.getElapsedTime();
    //sf::Time elapsed2 =clock1.getElapsedTime();
    DrawSceneTex();
    DrawBackground();
    //sf::Time elapsed3 =clock1.getElapsedTime();
    DrawLights();

    //sf::Time elapsed4 =clock1.getElapsedTime();
    texture_back->display();
    player->Draw2(*texture_back);
    sf::Sprite sprite(texture_back->getTexture());
    //sprite = map_without_lights;

    sf::Vector2f pos_sprite = GetPosSprite();
    sprite.setPosition(pos_sprite);
    sf::Vector2f oldOrgin = sprite.getOrigin();
    sprite.setOrigin(sprite.getTexture()->getSize().x/(2),sprite.getTexture()->getSize().y/(2));

    sprite.setScale(sf::Vector2f(zoom,zoom));
    sprite.setPosition(sprite.getPosition().x+sprite.getTexture()->getSize().x/(2), sprite.getPosition().y+sprite.getTexture()->getSize().y/(2));

    mix_back_terr_shader.setParameter("texture2", texture_background.getTexture());
    sf::RenderStates states;
    states.texture = &texture_back->getTexture();
    states.shader = &mix_back_terr_shader;
    //sf::Time elapsed5 =clock1.getElapsedTime();
    renderWindow.draw(sprite, states);
    //sf::Time elapsed6 =clock1.getElapsedTime();
    //float time1 = elapsed1.asSeconds();
    //float time2 = elapsed2.asSeconds()-elapsed1.asSeconds();
    //float time3 = elapsed3.asSeconds()-elapsed2.asSeconds();
    //float time4 = elapsed4.asSeconds()-elapsed3.asSeconds();
    //float time5 = elapsed5.asSeconds()-elapsed4.asSeconds();
    //float time6 = elapsed6.asSeconds()-elapsed5.asSeconds();
    //std::cout <<"create map without lights: " << time3/elapsed6.asSeconds()*100  << "%   draw lights: " << time4/elapsed6.asSeconds()*100 << "%   player: " << time5/elapsed6.asSeconds()*100 << "%   draw window: " << time6/elapsed6.asSeconds()*100 << "%"<< std::endl;

}

void Drawer::Draw(sf::RenderWindow &window, float zoom){
    //sf::Clock clock1;
    //clock1.restart().asSeconds();


    view_player.setCenter(player->GetPosition().x+(player->GetWidth()/2), player->GetPosition().y+(player->GetHeight()/2));

    texture_back->setView(view_player);
    texture_front->setView(view_player);
    texture_scene.setView(view_player);
    texture_sun.setView(view_player);
    texture_background.setView(view_player);
    black_texture.setView(view_player);
    //sf::Time elapsed1 =clock1.getElapsedTime();
    DrawMap(window, zoom);
    //sf::Time elapsed2 =clock1.getElapsedTime();
    player->DrawInventory(window);
    //sf::Time elapsed3 =clock1.getElapsedTime();
    //float time1 = elapsed1.asSeconds();
    //float time2 = elapsed2.asSeconds();
    //float time3 = elapsed3.asSeconds();
    //std::cout << time1 << " " << time2 << " " << time3  << std::endl;
}
