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
    sf::Texture texture3;
    sf::Sprite starship;
    std::vector <std::string> buffer;
    std::string id;
    float speed = 350.f;
    Game *game;

    Starship(Game *game, std::string id);
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
