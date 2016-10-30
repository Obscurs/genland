//
// Created by arnito on 30/10/16.
//

#include "Drawer.h"
#include "Game.h"
Drawer::Drawer(Map *m,Player *p,WorldBackground *b,Clock *c){
    map_curr = m;
    player = p;
    clock = c;
    backgrounds = b;
    texture_plain_sprite.create(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT);
    texture_front = new sf::RenderTexture();
    texture_back = new sf::RenderTexture();
    texture_front->create(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT);
    texture_back->create(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT);

    black_texture.create(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT);
    sf::RectangleShape rectangle(sf::Vector2f(0, 0));
    rectangle.setSize(sf::Vector2f(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT));
    rectangle.setFillColor(sf::Color::Black);
    black_texture.draw(rectangle);



    // Load the shaders
    if (!tile_shader.loadFromFile("resources/light2.frag", sf::Shader::Fragment)) std::cout<< "el shader no va" << std::endl;
    if (!sun_shader.loadFromFile("resources/sun.frag", sf::Shader::Fragment)) std::cout<< "el shader no va" << std::endl;
    if (!sun_mix_shader.loadFromFile("resources/sun_mix.frag", sf::Shader::Fragment)) std::cout<< "el shader no va" << std::endl;
    tile_shader.setParameter("windowHeight", static_cast<float>(Game::SCREEN_HEIGHT)); // this must be set, but only needs to be set once (or whenever the size of the window changes)
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
    int last_chunk = map_curr->getChunkIndex(last_x+Chunk::TILE_SIZE);
    for(int i = first_chunk ; i<=last_chunk ; ++i) {
        int index_mat = map_curr->getIndexMatChunk(i);
        map_curr->chunk_mat[index_mat]->DrawGrassTiles(*texMan,render_array);
    }
}

sf::Sprite Drawer::get_plain_sprite(sf::RenderWindow& renderWindow,sf::VertexArray &render_array,sf::VertexArray &sky_array){
    texture_plain_sprite.clear(sf::Color::Red);
    backgrounds->Draw(texture_plain_sprite);

    sf::View currentView = renderWindow.getView();
    sf::Vector2f centerView = currentView.getCenter();
    sf::Vector2f sizeView = currentView.getSize();
    float first_x = centerView.x-(sizeView.x/2)-1;
    float first_y = centerView.y-(sizeView.y/2)-1;
    float last_x = centerView.x+(sizeView.x/2)+1;
    float last_y = centerView.y+(sizeView.y/2)+1;

    sf::Vector2f firstPos(first_x, first_y);
    sf::Vector2f lastPos(last_x+Chunk::TILE_SIZE, last_y+Chunk::TILE_SIZE);
    int first_chunk = map_curr->getChunkIndex(first_x);
    int last_chunk = map_curr->getChunkIndex(last_x+Chunk::TILE_SIZE);

    for(int i = first_chunk ; i<=last_chunk ; ++i) {
        int index_mat = map_curr->getIndexMatChunk(i);
        map_curr->chunk_mat[index_mat]->DrawChunk(renderWindow, firstPos, lastPos, *texMan, tile_shader,render_array, sky_array);
    }

    for(int i = 0; i<map_curr->falling_tiles.size(); i++){

        map_curr->falling_tiles[i]->Draw(renderWindow, *texMan);
    }
    DrawFrontItemsMap(renderWindow,render_array);

    sf::RenderStates states;
    states.texture = texMan->getTexture();
    texture_plain_sprite.setView(currentView);
    texture_plain_sprite.draw(render_array, states);


    sf::Vector2f pos_sprite = firstPos;
    pos_sprite.x+=1;
    pos_sprite.y+=1;
    sf::Sprite map_without_lights(texture_plain_sprite.getTexture());
    map_without_lights.setPosition(pos_sprite);
    texture_plain_sprite.display();
    return map_without_lights;
}
void Drawer::DrawLights(sf::View& currentView,sf::VertexArray &render_array,sf::VertexArray &sky_array, sf::Sprite map_without_lights){
    sf::RenderStates states;
    states.texture = texMan->getTexture();

    //DRAWING SUN
    states.shader = &sun_shader;
    texture_back->clear(sf::Color::Black);
    texture_back->draw(sky_array, states);
    texture_back->display();

    sun_mix_shader.setParameter("color", sf::Color::Black);
    sun_mix_shader.setParameter("factor", 0.1);
    sun_mix_shader.setParameter("texture2", texture_back->getTexture());
    states.shader = &sun_mix_shader;
    texture_front->clear(sf::Color::Yellow);
    texture_front->setView(currentView);
    texture_front->draw(map_without_lights, states);
    texture_front->display();

    std::swap(texture_back,texture_front);

    //END DRAWING SUN
    //DRAWING MAP LIGHTS
    for(int i = 0; i<map_curr->lights.size(); i++){
        map_curr->lights[i].Draw(currentView,map_without_lights,tile_shader,texMan,texture_front,texture_back);
        std::swap(texture_back, texture_front);
    }
}

void Drawer::DrawMap(sf::RenderWindow& renderWindow)
{
    sf::View currentView = renderWindow.getView();
    sf::VertexArray render_array(sf::Quads , (uint)(4));
    sf::VertexArray sky_array(sf::Quads , (uint)(4));
    sf::Sprite map_without_lights = get_plain_sprite(renderWindow, render_array,sky_array);
    DrawLights(currentView,render_array,sky_array,map_without_lights);
    sf::Sprite sprite(texture_back->getTexture());
    sprite.setPosition(map_without_lights.getPosition());
    renderWindow.draw(sprite);

}

void Drawer::Draw(sf::RenderWindow &window){
    DrawMap(window);
    player->Draw(window);
    player->DrawInventory(window);
}
