//
// Created by cherkaoui on 10/11/18.
//

#include "JoinRoom.hpp"
#include "RequestManager.hpp"
#include "RType.hpp"

JoinRoom::JoinRoom(RType *rType) : rType(rType) {

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
    text.setString("Room name:");
    text.setCharacterSize(30);
    text.setPosition(250, 430);
    roomname.setFont(font);
    roomname.setString(str_roomname);
    roomname.setCharacterSize(50);
    roomname.setPosition(250, 467);
    create.setFont(font);
    create.setString("Join Room");
    create.setCharacterSize(70);
    create.setPosition(250, 610);
    pointer[0] = 442;
    pointer[1] = 620;
};

int JoinRoom::event(sf::Event event , sf::RenderWindow *window) {
    if((starship.getPosition().y < 550) && (event.type == sf::Event::TextEntered || event.text.unicode == 8)) {
        str_roomname = event_to_string(event, str_roomname);
        roomname.setString(str_roomname);
    }
    if (event.key.code == sf::Keyboard::Up) {
        index = (index - 1) < 0 ? 1 : (index - 1);
        starship.setPosition(positionx, pointer[index]);
    } else if (event.key.code == sf::Keyboard::Down) {
        index = (index + 1) > 1 ? 0 : (index + 1);
        starship.setPosition(positionx, pointer[index]);
    }

    if (event.key.code == sf::Keyboard::Return) {
        if (starship.getPosition().y > 610) {
            this->rType->network->request("JOIN_ROOM " + str_roomname, [this](Command &response) {
                std::cout << "Response  join room join: "<< response.toStr() << std::endl;
                if (response.getCommand() != "200") {
                    this->create.setString("Retry");
                } else {
                    this->rType->view = ROOM;
                }
            });
            return JOINROOM;
        }
    } else if (event.key.code == sf::Keyboard::Escape) {
        return LOBBY;
    } else {
        return JOINROOM;
    }
};