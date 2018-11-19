//
// Created by cherkaoui on 10/11/18.
//

#ifndef R_TYPE_CREATEROOM_HPP
#define R_TYPE_CREATEROOM_HPP

#include "Client.hpp"

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

    float x  = 0;
    float positionx = 30;
    float positiony = 442;

    int *pointer = new int[3];

    CreateRoom(){
        if (!texture.loadFromFile("./src/client/myasset/background.png")) {
            std::cout << "ERROR TEXTURE" << std::endl;
        }
        background = sf::Sprite(texture);
       background.setScale(1.5f, 1.5f);

        if (!texture2.loadFromFile("./src/client/myasset/r-type.png")) {
            std::cout << "ERROR TEXTURE" << std::endl;
        }
        logo = sf::Sprite(texture2);
        logo.setPosition(60, 70);
        if (!texture3.loadFromFile("./src/client/myasset/starship.png")) {
            std::cout << "ERROR TEXTURE" << std::endl;
        }
        starship = sf::Sprite(texture3);
        starship.setPosition(positionx, positiony);
        starship.setScale(2.0f, 2.0f);

        //text

        if (!font.loadFromFile("./src/client/myasset/space font.ttf")) {
            std::cout << "ERROR FONT" << std::endl;
        }
        text.setFont(font);
        text.setString("Room name:");
        text.setCharacterSize(30);
        text.setPosition(250, 430);

        roomname.setFont(font);
        roomname.setString(str_roomname);
        roomname.setCharacterSize(50);
        roomname.setPosition(250, 467);

        text2.setFont(font);
        text2.setString("player:");
        text2.setCharacterSize(70);
        text2.setPosition(200, 520);

        nbplayer.setFont(font);
        nbplayer.setString(std::to_string(int_nbplayer));
        nbplayer.setCharacterSize(70);
        nbplayer.setPosition(420, 520);


        create.setFont(font);
        create.setString("Join Room");
        create.setCharacterSize(70);
        create.setPosition(250, 610);


        pointer[0] = 442;
        pointer[1] = 550;
        pointer[2] = 620;
    };

    ~CreateRoom(){

    };


    int backspace = 0;
    std::string event_to_string(sf::Event event, std::string str){
        sf::Uint32 key = event.text.unicode;

        std::cout << "key "<< key << "c = " << backspace << std::endl;
        if (key >= 128 || key ==  27 || key == 13)
            return str;
        if (key ==   8) {
            if(backspace > 0 && backspace != 105)
                backspace = backspace * -1;
            return  str.substr(0, str.size() != 0 ? str.size() -1 : 0);
        }
        if(backspace < 0 && key == 105) {
            std::cout << "add " << backspace << std::endl;
            str += static_cast<char>((backspace * -1));
            backspace = key;
        } else
            backspace = key;

        return str += static_cast<char>(key);
    }


    int index = 0;
    int event(sf::Event event , sf::RenderWindow *window){


        if((starship.getPosition().y < 550) && (event.type == sf::Event::TextEntered || event.text.unicode == 8)) {
            str_roomname = event_to_string(event, str_roomname);
            roomname.setString(str_roomname);
        }

        if (starship.getPosition().y == 550)
        {
            if (event.key.code == sf::Keyboard::Left) {
                int_nbplayer = (int_nbplayer - 1);
                int_nbplayer = int_nbplayer == 0 ? 4 : int_nbplayer;
                nbplayer.setString(std::to_string(int_nbplayer));
            } else if (event.key.code == sf::Keyboard::Right) {
                int_nbplayer = (((int_nbplayer + 1) % 5));
                int_nbplayer = int_nbplayer == 0 ? 1 : int_nbplayer;
                nbplayer.setString(std::to_string(int_nbplayer));
            }
        }

        if (event.key.code == sf::Keyboard::Up) {
            index = (index - 1) < 0 ? 2 : (index - 1);
            starship.setPosition(positionx, pointer[index]);
        } else if (event.key.code == sf::Keyboard::Down) {
            index = (index + 1) > 2 ? 0 : (index + 1);
            starship.setPosition(positionx, pointer[index]);
        }

        if (event.key.code == sf::Keyboard::Return)
        {
            if (starship.getPosition().y > 610) {
                return ROOM;
            }
        }
        if (starship.getPosition().y < 550) {
            return CREATEROOM;
        }
        return CREATEROOM;
    }

    void draw(sf::RenderWindow *window){
        this->x -= 0.015;
        this->background.setPosition(this->x,0 );

        if (starship.getPosition().y  == pointer[2]) {
            text2.setColor(sf::Color::White);
            text.setColor(sf::Color::White);
            create.setColor(sf::Color::Red);
            create.setCharacterSize(70);
        }
        else if (starship.getPosition().y == 550) {
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
};

#endif //R_TYPE_CREATEROOM_HPP
