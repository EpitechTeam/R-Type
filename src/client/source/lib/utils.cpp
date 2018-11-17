//
// Created by cherkaoui on 10/11/18.
//
#include "client.hpp"


sf::Sprite createSprite(const std::string path)
{
    sf::Texture texture;

    texture.loadFromFile(path);
    return sf::Sprite(texture);
}

void print(std::string string){
    std::cout << string << std::endl;
}