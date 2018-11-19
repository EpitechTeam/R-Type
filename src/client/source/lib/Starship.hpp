//
// Created by cherkaoui on 16/11/18.
//

#ifndef R_TYPE_STARSHIP_HPP
#define R_TYPE_STARSHIP_HPP
#include "Client.hpp"
#include "Bullet.hpp"

class Starship {

public:
    sf::Texture texture3;
    sf::Sprite starship;
    std::vector <std::string> buffer;
    std::string id = "random";
    float speed = 350.f;

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
        if (event.key.code == sf::Keyboard::Escape) {
            return ROOM;
        }
        return MAP;
    }



    sf::Clock r;
    sf::Time elapsed_time;
    float  delay_msecond = 300.0f;


    void draw(sf::RenderWindow *window, float deltatime,  std::vector<Bullet*> *bullet) {
        window->draw(this->starship);

        elapsed_time += r.restart();
        sf::Time delay = sf::milliseconds(delay_msecond);

        if( elapsed_time >= delay ){
            if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) ) {
                sf::Vector2f position = starship.getPosition();
                position.x += 5;
                bullet->emplace_back(new Bullet(position, id, 1150));
                elapsed_time = sf::microseconds(0);
            }
        }

        if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Left ) && starship.getPosition().x > 0 ) {
            starship.setPosition(starship.getPosition().x -speed * deltatime, starship.getPosition().y);
        }
        if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Right ) && starship.getPosition().x < 1220) {
            starship.setPosition(starship.getPosition().x + speed * deltatime, starship.getPosition().y);
        }
        if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Up ) && starship.getPosition().y > 0) {
            starship.setPosition(starship.getPosition().x, starship.getPosition().y -speed * deltatime);
        }
        if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Down  ) && starship.getPosition().y < 690) {
            starship.setPosition(starship.getPosition().x, starship.getPosition().y +  speed * deltatime);
        }
    }
};

#endif //R_TYPE_STARSHIP_HPP
