//
// Created by cherkaoui on 18/11/18.
//

#ifndef R_TYPE_MOB_HPP
#define R_TYPE_MOB_HPP
#include "Client.hpp"
#include "Animation.hpp"
#define A_MOB1 new Animation(&_rectTexture, sf::Vector2u(33, 36), 0.2,sf::Vector2u(1,8), 0)
#define A_MOB2 new Animation(&_rectTexture, sf::Vector2u(33, 34), 0.2,sf::Vector2u(1,3), 0)
#define A_MOB3 new Animation(&_rectTexture, sf::Vector2u(61, 50), 0.5,sf::Vector2u(1,3), 50)

class Mob {

public:
    sf::Vector2f _position;
    std::string _id;
    sf::Texture _rectTexture;
    Animation *animation;
    sf::RectangleShape _rect;
    float range;

    Mob(sf::Vector2f position, const std::string &id, float range) {
        _position = position;
        this->range = range;
        _id = id;
        //print("crate mob: " + _id);
        switch(id[0]) {
            case '2':  _rect = sf::RectangleShape(sf::Vector2f(33 * 2, 34 * 2));
                _rect.setPosition(0, 0.f);
                if (!_rectTexture.loadFromFile("asset/mob2.png")) {
                    std::cout << "ERROR TEXTURE" << std::endl;
                }
                _rect.setTexture(&_rectTexture);
                _rect.setPosition(position.x, position.y);
                animation = A_MOB2;
                break;
            case '3':  _rect = sf::RectangleShape(sf::Vector2f(50 * 2, 50 * 2));
                _rect.setPosition(0, 0.f);
                if (!_rectTexture.loadFromFile("asset/mob3.png")) {
                    std::cout << "ERROR TEXTURE" << std::endl;
                }
                _rect.setTexture(&_rectTexture);
                _rect.setPosition(position.x, position.y);
                animation = A_MOB3;
                break;
            default:_rect = sf::RectangleShape(sf::Vector2f(33 * 2, 36 * 2));
                _rect.setPosition(0, 0.f);
                if (!_rectTexture.loadFromFile("asset/mob1.png")) {
                    std::cout << "ERROR TEXTURE" << std::endl;
                }
                _rect.setTexture(&_rectTexture);
                _rect.setPosition(position.x, position.y);
                animation = A_MOB1;
                break;
        }
        animation->setLoop(true);
    };

    ~Mob(){

    };

    int event(sf::Event event , sf::RenderWindow *window) {
        return MAP;
    }

    bool draw(sf::RenderWindow *window, float deltatime) {


       // _rect.move(-0.2f, 0);
        animation->Update(0, deltatime);
        _rect.setTextureRect(animation->uvRect);
        if(_rect.getPosition().x > 0){
            window->draw(_rect);
            return true;
        }
        return false;
    }
};

#endif //R_TYPE_MOB_HPP
