//
// Created by cherkaoui on 17/11/18.
//

#ifndef R_TYPE_BULLET_HPP
#define R_TYPE_BULLET_HPP
#include "client.hpp"
#include "Animation.hpp"
class Animation {

public:
    sf::IntRect uvRect;
    sf::Vector2u sizeframe;
    sf::Vector2u currentImage;
    sf::Texture *texture;
    float switchTime;
    float totalTime;
    sf::Vector2u frame;
    unsigned int from;
    bool loop = true;
    bool way = true;

public:
    Animation(sf::Texture *_texture = new sf::Texture, sf::Vector2u sizeframe = sf::Vector2u(16,5), float _switchTime = 0.5, sf::Vector2u frame = sf::Vector2u(0, 4),
              unsigned int from = 0) {
        this->sizeframe = sizeframe;
        this->switchTime = _switchTime;
        this->frame = frame;
        this->from = from;
        this->texture = _texture;
        totalTime = 0.0f;
        currentImage.x  = 0;
        uvRect.width = this->sizeframe.x;
        uvRect.height = this->sizeframe.y;
    }

    void setLoop(bool loop){
        this->loop = loop;
    }
    void setWay(bool way){
        this->way = way;
        this->currentImage.x = 0;
    }

    ~Animation(){

    }

    void Update(unsigned int row, float deltaTime) {
        currentImage.y = row;
        totalTime += deltaTime;
        if(totalTime >=  switchTime) {
            totalTime -= switchTime;
            if(way)
            {
                if(currentImage.x != frame.y) {
                    currentImage.x++;
                } else if(loop) {
                    currentImage.x = 0;
                }
            } else {
                if(currentImage.x != 0) {
                    currentImage.x--;
                } else if(loop) {
                    currentImage.x = frame.y;
                }
            }

        }

        uvRect.left = texture->getSize().x - currentImage.x * uvRect.width;
        uvRect.top = from;
        /*
        std::cout << uvRect.left << " <= left" << std::endl;
        std::cout << uvRect.top << " <= top" << std::endl;*/
    }
};


class Bullet {

public:
    sf::Texture texture;
    sf::Sprite bullet;
    sf::Vector2f _position;
    std::string _id;
    sf::Texture _rectTexture;
    Animation *animation;
    sf::RectangleShape _rect;



    Bullet(sf::Vector2f position, std::string id) {
        if (!texture.loadFromFile("./src/client/myasset/shoot.png")) {
            std::cout << "ERROR TEXTURE" << std::endl;
        }
        _position = position;
        _id = id;

        bullet = sf::Sprite(texture);

        bullet.setPosition(position.x, position.y);

        bullet.setScale(0.3f, 0.3f);

        _rect = sf::RectangleShape(sf::Vector2f(32, 32));
        _rect.setPosition(0, 0.f);
        if (!_rectTexture.loadFromFile("./src/client/asset/r-typesheet2.png")) {
            std::cout << "ERROR TEXTURE" << std::endl;
        }
        _rect.setTexture(&_rectTexture);
        _rect.setPosition(position.x, position.y);
        //animation = new Animation(&_rectTexture, sf::Vector2u(66, 35), 0.1,sf::Vector2u(0,4), 468); // infinity
        animation = new Animation(&_rectTexture, sf::Vector2u(33, 32), 0.1,sf::Vector2u(0,4), 0); // infinity
        animation->setLoop(false);
    };

    ~Bullet(){

    };

    int event(sf::Event event , sf::RenderWindow *window) {
        return MAP;
    }
    void draw(sf::RenderWindow *window, float deltatime) {
        _rect.move(0.5, 0);
        animation->Update(0, deltatime);
        _rect.setTextureRect(animation->uvRect);
        window->draw(_rect);
    }
};

#endif //R_TYPE_BULLET_HPP
