//
// Created by cherkaoui on 17/11/18.
//

#ifndef R_TYPE_ANIMATION_HPP
#define R_TYPE_ANIMATION_HPP
#include "client.hpp"
#include <SFML/System.hpp>
#include <SFML/Graphics/Texture.hpp>

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
        currentImage.x  = this->frame.x;
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
                    currentImage.x = frame.x;
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

    }
};


#endif //R_TYPE_ANIMATION_HPP
