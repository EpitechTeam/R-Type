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



    Auth(){
        if (!texture.loadFromFile("./src/client/myasset/background.jpg")) {
            std::cout << "ERROR TEXTURE" << std::endl;
        }
        background = sf::Sprite(texture);
        background.setScale(1.9f, 1.4f);

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
        starship.setScale(0.3f, 0.3f);

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
        text2.setPosition(250, 530);

        nbplayer.setFont(font);
        nbplayer.setString(server_ip);
        nbplayer.setCharacterSize(30);
        nbplayer.setPosition(375, 542);

        create.setFont(font);
        create.setString("Connect");
        create.setCharacterSize(40);
        create.setPosition(250, 610);
    };

    ~Auth(){

    };

    void handleTextEntered(sf::Uint32 key) {
        if (key >= 128 || key ==  27 || key == 13)
            return;
        if (key ==   8) {
            str_roomname = str_roomname.substr(0, str_roomname.size() != 0 ? str_roomname.size() -1 : 0);
            return;
        }
        str_roomname += static_cast<char>(key);
    }

    void handleIpEntered(sf::Uint32 key) {
        if (key >= 128 || key ==  27 || key == 13)
            return;
        if (key ==   8) {
            server_ip = server_ip.substr(0, server_ip.size() != 0 ? server_ip.size() -1 : 0);
            return;
        }
        server_ip += static_cast<char>(key);
    }

    int event(sf::Event event , sf::RenderWindow *window){


        if((starship.getPosition().y < 530) && (event.type == sf::Event::TextEntered || event.text.unicode == 8)) {
            handleTextEntered(event.text.unicode);
            roomname.setString(str_roomname);
        } else if((starship.getPosition().y < 575) && (event.type == sf::Event::TextEntered || event.text.unicode == 8)) {
            handleIpEntered(event.text.unicode);
            nbplayer.setString(server_ip);
        }

        if (event.key.code == sf::Keyboard::Left) {
            starship.move(-3.f, 0.f);
        }
        else if (event.key.code == sf::Keyboard::Right) {
            starship.move(3.f, 0.f);
        }
        else if (event.key.code == sf::Keyboard::Down) {
            starship.move(0.f, 3.f);
        }
        else if (event.key.code == sf::Keyboard::Up) {
            starship.move(0.f, -3.f);
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
            create.setCharacterSize(42);
        }
        else if (starship.getPosition().y > 530) {
            create.setColor(sf::Color::White);
            text2.setColor(sf::Color::Red);
            text2.setCharacterSize(42);
            text.setColor(sf::Color::White);
            text.setCharacterSize(30);
        } else {
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
