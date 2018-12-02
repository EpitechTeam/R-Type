//
// Created by cherkaoui on 16/11/18.
//

#ifndef R_TYPE_STARSHIP_HPP
#define R_TYPE_STARSHIP_HPP
#include "Client.hpp"
#include "Bullet.hpp"

class Game;

class Starship {

public:
    Game *game;
    std::string id;
    sf::Texture texture3;
    sf::Sprite starship;
    std::vector <std::string> buffer;
    float speed = 350.f;

    Starship(Game *game, std::string id, int color);
    ~Starship(){};
    int event(sf::Event event , std::vector<Bullet*> *bullet, sf::RenderWindow *window) {
        if (event.key.code == sf::Keyboard::Escape) {
            return ROOM;
        }
        return MAP;
    }
    sf::Clock r;
    sf::Time elapsed_time;
    float  delay_msecond = 100.0f;
    void draw(sf::RenderWindow *window, float deltatime,  std::vector<Bullet*> *bullet);
};

#endif //R_TYPE_STARSHIP_HPP
