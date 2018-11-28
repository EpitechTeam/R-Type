//
// Created by cherkaoui on 11/11/18.
//

#include "Auth.hpp"
#include "RequestManager.hpp"
#include "RType.hpp"

Auth::Auth(RType *rType) : rType(rType) {
    if (!texture.loadFromFile("myasset/background.png")) {
        std::cout << "ERROR TEXTURE" << std::endl;
    }
    background = sf::Sprite(texture);
    background.setScale(1.5f, 1.5f);

    if (!texture2.loadFromFile("myasset/r-type.png")) {
        std::cout << "ERROR TEXTURE" << std::endl;
    }
    logo = sf::Sprite(texture2);
    logo.setPosition(60, 70);
    if (!texture3.loadFromFile("myasset/starship.png")) {
        std::cout << "ERROR TEXTURE" << std::endl;
    }
    starship = sf::Sprite(texture3);
    starship.setPosition(positionx, positiony);
    starship.setScale(2.0f, 2.0f);
    if (!font.loadFromFile("myasset/space font.ttf")) {
        std::cout << "ERROR FONT" << std::endl;
    }
    text.setFont(font);
    text.setString("Nickname: ");
    text.setCharacterSize(30);
    text.setPosition(250, 430);
    roomname.setFont(font);
    roomname.setString(playername);
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

int Auth::event(sf::Event event , sf::RenderWindow *window) {
    if((starship.getPosition().y < 530) && (event.type == sf::Event::TextEntered || event.text.unicode == 8)) {
        playername = event_to_string(event, playername);
        roomname.setString(playername);
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
    if (event.key.code == sf::Keyboard::Return) {

        if (starship.getPosition().y > 575) {
            return  AUTH_CONNECT;
        }
    }
    if (starship.getPosition().y < 530) {
        return AUTH;
    }
    return AUTH;
}

void Auth::draw(sf::RenderWindow *window){
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