//
// Created by cherkaoui on 10/11/18.
//
#include "Client.hpp"
#include <cctype>


sf::Sprite createSprite(const std::string path)
{
    sf::Texture texture;

    texture.loadFromFile(path);
    return sf::Sprite(texture);
}

void print(std::string string){
    std::cout << string << std::endl;
};


int roll(int min, int max) {
    return 4;
}


bool check_ip(std::string serverip) {
    int points = 0;
    int doublepoints = 0;
    unsigned int i = 0;
    int number = 0;
    while(i != serverip.size())
    {
        if(serverip[i] == '.'){
            points++;
            if(points > 3)
                return false;
        } else if( serverip[i] == ':') {
            doublepoints++;
            if(doublepoints > 1)
                return false;
        }
        else if (isalnum(serverip[i])){
            number++;
            std::cout << "is Number man: " << serverip[i] << std::endl;
        } else
            return false;
        i++;
    }
    return true;
}