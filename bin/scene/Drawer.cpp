//
// Created by arnito on 30/10/16.
//

#include "Drawer.h"
#include "../Game.h"
#include "../Settings.h"
#include "../Resources.h"
#include "../Debuger.h"

Drawer::Drawer(Map *m,Player *p,WorldBackground *b,Clock *c)
{
    texture_scene.create(Settings::GAME_WIDTH, Settings::GAME_HEIGHT);
    texture_background.create(Settings::GAME_WIDTH, Settings::GAME_HEIGHT);
    texture_sun.create(Settings::GAME_WIDTH, Settings::GAME_HEIGHT);
    texture_front = new sf::RenderTexture();
    texture_back = new sf::RenderTexture();
    timer.restart();

    view_player.setSize(Settings::GAME_WIDTH, Settings::GAME_HEIGHT);
    view_player.setSize(Settings::GAME_WIDTH, Settings::GAME_HEIGHT);
    //view_player._zoom(0.5);
    texture_front->create(Settings::GAME_WIDTH, Settings::GAME_HEIGHT);
    texture_back->create(Settings::GAME_WIDTH, Settings::GAME_HEIGHT);
    black_texture.create(Settings::GAME_WIDTH, Settings::GAME_HEIGHT);
    sf::RectangleShape rectangle(sf::Vector2f(0, 0));
    rectangle.setSize(sf::Vector2f(Settings::GAME_WIDTH, Settings::GAME_HEIGHT));
    rectangle.setFillColor(sf::Color::Black);
    black_texture.draw(rectangle);

    sf::Shader *s2 = Resources::getShader("tile_shader");
    sf::Shader *s3 = Resources::getShader("rain_shader");
    s2->setParameter("windowHeight", static_cast<float>(Settings::GAME_HEIGHT));
    s3->setParameter("windowHeight", static_cast<float>(Settings::GAME_HEIGHT));
    s3->setParameter("windowWidth", static_cast<float>(Settings::GAME_WIDTH));



}


Drawer::~Drawer()
{

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
    TextureManager *texMan = Resources::getTextureManager("tileMap");
    Scene *scene = Scene::getScene();
    Map &map_curr = scene->getMap();
    texture_scene.clear(sf::Color(0,0,0,0));
    sf::RenderStates states;
    states.texture = texMan->getTexture();

    sf::Vector2f centerView = view_player.getCenter();
    sf::Vector2f sizeView = view_player.getSize();
    float first_x = centerView.x-(sizeView.x/2)-1;
    float last_x = centerView.x+(sizeView.x/2)+1;
    int first_chunk = map_curr.getChunkIndex(first_x);
    int last_chunk = map_curr.getChunkIndex(last_x+Settings::TILE_SIZE);
    for(int i = first_chunk ; i<=last_chunk ; i = i +1) {
        int index_mat = map_curr.getIndexMatChunk(i);
        texture_scene.draw(map_curr._chunk_mat[index_mat]->render_array, states);
    }
    for(int i = 0; i<map_curr.falling_tiles.size(); i++){
        map_curr.falling_tiles[i]->Draw(texture_scene);
    }
    texture_scene.display();
}
void Drawer::DrawBackground() {
    sf::Shader *sun_background_shader = Resources::getShader("sun_background_shader");
    Scene *scene = Scene::getScene();
    Clock &clock = scene->getClock();
    WorldBackground &backgrounds = scene->getBackgrounds();
    clock.setColorToShader(*sun_background_shader);

    texture_background.clear(sf::Color(255,0,0,255));

    backgrounds.Draw(texture_background);

    sf::Sprite background_sprite(texture_background.getTexture());
    sf::Vector2f pos_sprite = GetPosSprite();
    background_sprite.setPosition(pos_sprite);

    sf::RenderStates states;
    texture_background.display();
    states.texture = &texture_background.getTexture();
    states.shader = sun_background_shader;
    texture_background.draw(background_sprite, states);
    texture_background.display();
}

void Drawer::DrawRain() {
    Scene *scene = Scene::getScene();
    Clock &clock = scene->getClock();
    Map &map_curr = scene->getMap();
    Player &player = scene->getPlayer();
    if(clock._rainFactor >0){
        sf::Shader *rain_shader = Resources::getShader("rain_shader");
        sf::Sprite background_sprite(texture_background.getTexture());
        sf::Vector2f pos_sprite = GetPosSprite();
        background_sprite.setPosition(pos_sprite);
        rain_shader->setParameter("time",timer.getElapsedTime().asSeconds());

        rain_shader->setParameter("intensity",clock._rainFactor);
        //rain_shader.setParameter("LAYERS",int(_clock->_rainFactor*10/2));


        sf::Vector2f pos = player.GetPosition();
        Tile *t = map_curr.getTile(pos.x,pos.y,0);
        int temperature = t->_temperature;
        int humidity = t->_humidity;
        sf::RenderStates states;
        texture_background.display();
        states.texture = &texture_background.getTexture();
        states.shader = rain_shader;
        if(temperature >10){
            if(temperature >25 && humidity<50){
                float new_rain_factor = clock._rainFactor-1-(humidity-50)/10;
                rain_shader->setParameter("intensity",new_rain_factor);
            }
            rain_shader->setParameter("SCALE",512.0f);
            rain_shader->setParameter("LENGTH",64.0f);
            rain_shader->setParameter("LENGTH_SCALE",0.8f);
            rain_shader->setParameter("SPEED",10.0f);
            rain_shader->setParameter("ALPHA",1.f);
            texture_background.draw(background_sprite, states);
            texture_background.display();
        } else if(temperature < -10){
            rain_shader->setParameter("SCALE",256.0f);
            rain_shader->setParameter("LENGTH",2.0f);
            rain_shader->setParameter("LENGTH_SCALE",0.8f);
            rain_shader->setParameter("SPEED",1.0f);
            rain_shader->setParameter("ALPHA",1.f);
            texture_background.draw(background_sprite, states);
            texture_background.display();
        } else{
            float factor = float(temperature-(-10))/20.0f;
            rain_shader->setParameter("SCALE",256.0f);
            rain_shader->setParameter("LENGTH",2.0f);
            rain_shader->setParameter("LENGTH_SCALE",0.8f);
            rain_shader->setParameter("SPEED",1.0f);
            rain_shader->setParameter("ALPHA",1.0f-factor);
            texture_background.draw(background_sprite, states);
            texture_background.display();

            rain_shader->setParameter("SCALE",512.0f);
            rain_shader->setParameter("LENGTH",64.0f);
            rain_shader->setParameter("LENGTH_SCALE",0.8f);
            rain_shader->setParameter("SPEED",10.0f);
            rain_shader->setParameter("ALPHA",factor);
            texture_background.draw(background_sprite, states);
            texture_background.display();
        }






    }

}
void Drawer::DrawLights(){
    TextureManager *texMan = Resources::getTextureManager("tileMap");
    sf::Shader *sun_shader = Resources::getShader("sun_shader");
    sf::Shader *sun_mix_shader = Resources::getShader("sun_mix_shader");
    Scene *scene = Scene::getScene();
    Clock &clock = scene->getClock();
    Map &map_curr = scene->getMap();

    sf::Sprite map_without_lights(texture_scene.getTexture());
    sf::Vector2f pos_sprite = GetPosSprite();
    map_without_lights.setPosition(pos_sprite);
    sf::RenderStates states;
    states.texture = texMan->getTexture();

    //DRAWING SUN
    states.shader = sun_shader;
    texture_sun.clear(sf::Color(0,0,0,0));
    sf::Vector2f centerView = view_player.getCenter();
    sf::Vector2f sizeView = view_player.getSize();
    float first_x = centerView.x-(sizeView.x/2)-1;
    float first_y = centerView.y-(sizeView.y/2)-1;
    float last_x = centerView.x+(sizeView.x/2)+1;
    float last_y = centerView.y+(sizeView.y/2)+1;
    int first_chunk = map_curr.getChunkIndex(first_x);
    int last_chunk = map_curr.getChunkIndex(last_x+Settings::TILE_SIZE);

    for(int i = first_chunk ; i<=last_chunk ; i++) {
        int index_mat = map_curr.getIndexMatChunk(i);
        texture_sun.draw(map_curr._chunk_mat[index_mat]->sky_array, states);

    }
    texture_sun.display();
    clock.setColorToShader(*sun_mix_shader);
    sun_mix_shader->setParameter("texture2", texture_sun.getTexture());
    states.shader = sun_mix_shader;
    texture_back->clear(sf::Color(0,0,0,0));
    texture_front->clear(sf::Color(0,0,0,0));
    texture_front->draw(map_without_lights, states);
    texture_front->display();

    std::swap(texture_back,texture_front);

    //END DRAWING SUN
    //DRAWING MAP LIGHTS
    //texture_back->setView(currentView);
    //texture_front->setView(currentView);


    for(int i = 0; i<map_curr.lights.size(); i++){
        //for(int i = 0; i<4; i++){
        float radius = map_curr.lights[i].radius;
        sf::Vector2f lightpos = sf::Vector2f(map_curr.lights[i].position.x-first_x,map_curr.lights[i].position.y-first_y);
        if(lightpos.x>0-radius*2 && lightpos.x<sizeView.x+radius*2 && lightpos.y>0-radius*2 && lightpos.y<sizeView.y+radius*2) {

            map_curr.lights[i].Draw(lightpos, map_without_lights, texture_front, texture_back,texture_sun);
            std::swap(texture_back, texture_front);
        }
    }

}
void Drawer::debugMap(const std::string keyDebug){
    Scene *scene = Scene::getScene();
    Map &map_curr = scene->getMap();
    sf::Text text;
    text.setFont(*Resources::getFont("debugFont"));
    text.setCharacterSize(12);
    text.setColor(sf::Color::Red);
    for(int i = 0 ; i<Map::N_CHUNKS_X ; i = i +1) {
        map_curr._chunk_mat[i]->debugDraw(*texture_back, keyDebug,text);
    }
}
void Drawer::DrawMap(sf::RenderWindow& renderWindow)
{
    sf::Shader *mix_back_terr_shader = Resources::getShader("mix_back_terr_shader");
    Scene *scene = Scene::getScene();
    Player &player = scene->getPlayer();
    float zoom = scene->getZoom();
    DrawSceneTex();
    DrawBackground();
    DrawRain();
    DrawLights();
    if(Debuger::activated && Debuger::metric1 !="none") debugMap(Debuger::metric1);
    if(Debuger::activated && Debuger::metric2 !="none") debugMap(Debuger::metric2);
    if(Debuger::activated && Debuger::metric3 !="none") debugMap(Debuger::metric3);

    texture_back->display();
    player.Draw2(*texture_back);
    sf::Sprite sprite(texture_back->getTexture());

    sf::Vector2f pos_sprite = GetPosSprite();
    sprite.setPosition(pos_sprite);
    sprite.setOrigin(sprite.getTexture()->getSize().x/(2),sprite.getTexture()->getSize().y/(2));

    sprite.setScale(sf::Vector2f(zoom,zoom));
    sprite.setPosition(sprite.getPosition().x+sprite.getTexture()->getSize().x/(2), sprite.getPosition().y+sprite.getTexture()->getSize().y/(2));

    mix_back_terr_shader->setParameter("texture2", texture_background.getTexture());
    sf::RenderStates states;
    states.texture = &texture_back->getTexture();
    states.shader = mix_back_terr_shader;
    renderWindow.draw(sprite, states);
}

void Drawer::Draw(sf::RenderWindow &window){
    Scene *scene = Scene::getScene();
    Map &map_curr = scene->getMap();
    Player &player = scene->getPlayer();
    view_player.setCenter(player.GetPosition().x+player.GetWidth()/2, player.GetPosition().y+player.GetHeight()/2);

    texture_back->setView(view_player);
    texture_front->setView(view_player);
    texture_scene.setView(view_player);
    texture_sun.setView(view_player);
    texture_background.setView(view_player);
    black_texture.setView(view_player);
    DrawMap(window);
    player.DrawInventory(window);
    map_curr.drawViewMap(window);

}
