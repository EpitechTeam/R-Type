//
// Created by cherkaoui on 10/11/18.
//

#ifndef R_TYPE_CREATEROOM_HPP
#define R_TYPE_CREATEROOM_HPP

class RequestManager;

#include <Command.hpp>
#include "Client.hpp"

class RType;

class CreateRoom {

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
    std::string str_roomname = "my_room";
    int int_nbplayer = 4;

    float x = 0;
    float positionx = 30;
    float positiony = 442;

    int *pointer = new int[3];

    RType *rType;

    CreateRoom(RType *rType);

    ~CreateRoom() {

    };


    int backspace = 0;

    std::string event_to_string(sf::Event event, std::string str) {
        sf::Uint32 key = event.text.unicode;

        std::cout << "key " << key << "c = " << backspace << std::endl;
        if (key >= 128 || key == 27 || key == 13)
            return str;
        if (key == 8) {
            if (backspace > 0 && backspace != 105)
                backspace = backspace * -1;
            return str.substr(0, str.size() != 0 ? str.size() - 1 : 0);
        }
        if (backspace < 0 && key == 105) {
            std::cout << "add " << backspace << std::endl;
            str += static_cast<char>((backspace * -1));
            backspace = key;
        } else
            backspace = key;

        return str += static_cast<char>(key);
    }


    int index = 0;

    void draw(sf::RenderWindow *window) {
        this->x -= 0.015;
        this->background.setPosition(this->x, 0);

        if (starship.getPosition().y == pointer[2]) {
            text2.setColor(sf::Color::White);
            text.setColor(sf::Color::White);
            create.setColor(sf::Color::Red);
            create.setCharacterSize(70);
        } else if (starship.getPosition().y == 550) {
            create.setColor(sf::Color::White);
            text2.setColor(sf::Color::Red);
            text2.setCharacterSize(72);
            text.setColor(sf::Color::White);
            roomname.setColor(sf::Color::White);
            text.setCharacterSize(30);
        } else {
            create.setColor(sf::Color::White);
            text.setColor(sf::Color::Red);
            roomname.setColor(sf::Color::White);
            text.setCharacterSize(35);
            text2.setColor(sf::Color::White);
            text2.setCharacterSize(70);
        }
        window->draw(this->background);
        window->draw(this->text);
        window->draw(this->text2);
        window->draw(this->nbplayer);
        window->draw(this->roomname);
        window->draw(this->create);
        window->draw(this->logo);
        window->draw(this->starship);
    }

    int event(sf::Event event, sf::RenderWindow *window);
};

#endif //R_TYPE_CREATEROOM_HPP
