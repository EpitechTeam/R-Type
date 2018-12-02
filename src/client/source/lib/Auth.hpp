//
// Created by cherkaoui on 11/11/18.
//

#ifndef R_TYPE_AUTH_HPP
#define R_TYPE_AUTH_HPP

class RequestManager;

#include <Command.hpp>
#include "Client.hpp"

class RType;

class Auth {
public:
    sf::Texture texture;
    sf::Sprite background;
    sf::Texture texture2;
    sf::Sprite logo;
    sf::Texture texture3;
    sf::Sprite starship;
    sf::Font font;
    sf::Text text;
    sf::Text text2;
    sf::Text nbplayer;
    sf::Text roomname;
    sf::Text create;
    std::string playername = "guest" +  std::to_string((rand() % 400 + 1));
    //std::string  server_ip = "192.168.1.98:4242";
    std::string  server_ip = "127.0.0.1:4242";
    float x  = 0;
    float positionx = 30;
    float positiony = 442;
    int *pointer = new int[3];
    int index = 0;
    int backspace = 0;
    RType *rType;

    Auth(RType *rType);
    ~Auth(){};

    std::string event_to_string(sf::Event event, std::string str){
        sf::Uint32 key = event.text.unicode;
        if (key >= 128 || key ==  27 || key == 13)
            return str;
        if (key ==   8) {
            if(backspace > 0 && backspace != 105)
            backspace = backspace * -1;
            return  str.substr(0, str.size() != 0 ? str.size() -1 : 0);
        }
        if(backspace < 0 && key == 105) {
            str += static_cast<char>((backspace * -1));
            backspace = key;
        } else
            backspace = key;
        return str += static_cast<char>(key);
    }

    int event(sf::Event event , sf::RenderWindow *window);
    void draw(sf::RenderWindow *window);
};

#endif //R_TYPE_AUTH_HPP
