//
// Created by cherkaoui on 10/11/18.
//

#include "CreateRoom.hpp"
#include "RequestManager.hpp"
#include "RType.hpp"

CreateRoom::CreateRoom(RType *rType)
        : rType(rType) {

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

int
CreateRoom::event(sf::Event event , sf::RenderWindow *window){


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

    if (event.key.code == sf::Keyboard::Return) {
        if (starship.getPosition().y > 610) {
            this->rType->network->request("CREATE_ROOM " + str_roomname + " " + std::to_string(int_nbplayer), [this](Command &response) {
                std::cout << "Response create: "<< response.toStr() << std::endl;
            });
            this->rType->network->request("JOIN_ROOM " + str_roomname, [this](Command &response) {
                std::cout << "Response join: "<< response.toStr() << std::endl;
                this->response_create_room = response.toStr();
                if (this->response_create_room != "200 ROOM_JOINED") {
                    this->create.setString("Retry");
                } else
                    this->rType->view = ROOM;
            });
            return CREATEROOM;
        }
    }
    std::cout << "Res join: "<< this->response_create_room << std::endl;
    if (this->response_create_room == "200 ROOM_JOINED") {
        this->response_create_room = "404";
        return ROOM;
    }
    else
    return CREATEROOM;
}