//
// Created by cherkaoui on 17/11/18.
//

#ifndef R_TYPE_BULLET_HPP
#define R_TYPE_BULLET_HPP
#include "Client.hpp"
#include "Animation.hpp"

class Bullet {

public:
    sf::Texture texture;
    sf::Sprite bullet;
    sf::Vector2f _position;
    std::string _id;
    sf::Texture _rectTexture;
    Animation *animation;
    sf::RectangleShape _rect;
    float  range;
    int dir = 1;



    Bullet(sf::Vector2f position, std::string id, float range, int dir = 1) {
        this->dir = dir;
        if (!texture.loadFromFile("myasset/shoot.png")) {
            std::cout << "ERROR TEXTURE" << std::endl;
        }
        this->range = range;
        _position = position;
        _id = id;

        bullet = sf::Sprite(texture);

        bullet.setPosition(position.x, position.y);

        bullet.setScale(0.3f, 0.3f);

        _rect = sf::RectangleShape(sf::Vector2f(32, 32));
        _rect.setPosition(0, 0.f);
        if (!_rectTexture.loadFromFile("asset/r-typesheet2.png")) {
            std::cout << "ERROR TEXTURE" << std::endl;
        }
        _rect.setTexture(&_rectTexture);
        _rect.setPosition(position.x, position.y);
        //animation = new Animation(&_rectTexture, sf::Vector2u(66, 35), 0.1,sf::Vector2u(0,4), 468); // infinity
        animation = new Animation(&_rectTexture, sf::Vector2u(33, 32), 0.1,sf::Vector2u(1,4), 0); // infinity
        animation->setLoop(false);
    };

    ~Bullet(){

    };

    int event(sf::Event event , sf::RenderWindow *window) {
        return MAP;
    }
    bool draw(sf::RenderWindow *window, float deltatime) {
        _rect.move(1 * this->dir, 0);
        animation->Update(0, deltatime);
        _rect.setTextureRect(animation->uvRect);
        if( (_rect.getPosition().x -  _position.x) <= range){
            window->draw(_rect);
            return true;
        }
        return false;
    }
};

#endif //R_TYPE_BULLET_HPP
