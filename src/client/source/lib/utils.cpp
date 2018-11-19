//
// Created by cherkaoui on 10/11/18.
//
#include "Client.hpp"


sf::Sprite createSprite(const std::string path)
{
    sf::Texture texture;

    texture.loadFromFile(path);
    return sf::Sprite(texture);
}

void print(std::string string){
    std::cout << string << std::endl;
}

int roll(int min, int max)
{
    double x = rand()/static_cast<double>(RAND_MAX+1);
    int that = min + static_cast<int>( x * (max - min));

    return that;
}