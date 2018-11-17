//
// Created by cherkaoui on 16/11/18.
//

#ifndef R_TYPE_STARSHIP_HPP
#define R_TYPE_STARSHIP_HPP
#include "client.hpp"
#include "Bullet.hpp"

class Starship {

public:
    sf::Texture texture3;
    sf::Sprite starship;
    std::vector <std::string> buffer;
    std::string id = "random";

    Starship(){
        if (!texture3.loadFromFile("./src/client/myasset/starship.png")) {
            std::cout << "ERROR TEXTURE" << std::endl;
        }
        starship = sf::Sprite(texture3);
        starship.setPosition(310, 10);
        starship.setScale(2.0f, 2.0f);
    };

    ~Starship(){

    };

    int event(sf::Event event , std::vector<Bullet*> *bullet, sf::RenderWindow *window){
        if (event.key.code == sf::Keyboard::Left) {
            starship.move(-5, 0);
        }
        else if (event.key.code == sf::Keyboard::Right) {
            starship.move(5, 0);
        }
        else if (event.key.code == sf::Keyboard::Up) {
            starship.move(0, -5);

        }
        else if (event.key.code == sf::Keyboard::Down) {
            starship.move(0, 5);
        }
        else if (event.key.code == sf::Keyboard::Escape) {
            return ROOM;
        }
        else if (event.key.code == sf::Keyboard::Space) {
            sf::Vector2f position = starship.getPosition();
            position.x += 5;
            bullet->emplace_back(new Bullet(position, id));
        }
        return MAP;
    }


    void draw(sf::RenderWindow *window) {
        window->draw(this->starship);

    }
};

#endif //R_TYPE_STARSHIP_HPP
