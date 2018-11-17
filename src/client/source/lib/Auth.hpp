//
// Created by cherkaoui on 11/11/18.
//

#ifndef R_TYPE_AUTH_HPP
#define R_TYPE_AUTH_HPP

#include "client.hpp"

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
    std::string str_roomname = "guest" +  std::to_string((rand() % 400 + 1));
    std::string  server_ip = "127.0.0.1:4242";

    float x  = 0;
    float positionx = 30;
    float positiony = 442;
    int *pointer = new int[3];


    Auth(){
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
        text.setString("Nickname: ");
        text.setCharacterSize(30);
        text.setPosition(250, 430);

        roomname.setFont(font);
        roomname.setString(str_roomname);
        roomname.setCharacterSize(50);
        roomname.setPosition(250, 467);

        text2.setFont(font);
        text2.setString("ip server:");
        text2.setCharacterSize(70);
        text2.setPosition(200, 530);

        nbplayer.setFont(font);
        nbplayer.setString(server_ip);
        nbplayer.setCharacterSize(30);
        nbplayer.setPosition(375, 542);

        create.setFont(font);
        create.setString("Connect");
        create.setCharacterSize(80);
        create.setPosition(110, 610);


        pointer[0] = 442;
        pointer[1] = 550;
        pointer[2] = 638;
    };

    ~Auth(){

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

    int event(sf::Event event , sf::RenderWindow *window) {

        if((starship.getPosition().y < 530) && (event.type == sf::Event::TextEntered || event.text.unicode == 8)) {
            str_roomname = event_to_string(event, str_roomname);
            roomname.setString(str_roomname);
        } else if((starship.getPosition().y < 575) && (event.type == sf::Event::TextEntered || event.text.unicode == 8)) {
            server_ip = event_to_string(event, server_ip);
            nbplayer.setString(server_ip);
        }

        if (event.key.code == sf::Keyboard::Up) {
            index = (index - 1) < 0 ? 2 : (index - 1);
            starship.setPosition(positionx, pointer[index]);
        }
        else if (event.key.code == sf::Keyboard::Down) {
            index = (index + 1) > 2 ? 0 : (index + 1);
            starship.setPosition(positionx, pointer[index]);
        }


        if (event.key.code == sf::Keyboard::Return)
        {

            if (starship.getPosition().y > 575) {
                return LOBBY;
            }
        }
        if (starship.getPosition().y < 530) {
            return AUTH;
        }
        return AUTH;
    }

    void draw(sf::RenderWindow *window){
        this->x -= 0.015;
        this->background.setPosition(this->x,0 );

        if (starship.getPosition().y > 575) {
            text2.setColor(sf::Color::White);
            text.setColor(sf::Color::White);
            create.setColor(sf::Color::Red);
            create.setCharacterSize(80);
        }
        else if (starship.getPosition().y > 530) {
            create.setCharacterSize(60);
            create.setColor(sf::Color::White);
            text2.setColor(sf::Color::Red);
            text2.setCharacterSize(42);
            text.setColor(sf::Color::White);
            text.setCharacterSize(30);
        } else {
            create.setCharacterSize(60);
            create.setColor(sf::Color::White);
            text.setColor(sf::Color::Red);
            text.setCharacterSize(35);
            text2.setColor(sf::Color::White);
            text2.setCharacterSize(40);
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

#endif //R_TYPE_AUTH_HPP
