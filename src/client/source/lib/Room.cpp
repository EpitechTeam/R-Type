//
// Created by cherkaoui on 11/11/18.
//

#include "Room.hpp"
#include "RequestManager.hpp"
#include "RType.hpp"

Room::Room(RType *rType) : rType(rType) {
    if (!texture.loadFromFile("myasset/background.png")) {
        std::cout << "ERROR TEXTURE" << std::endl;
    }
    background = sf::Sprite(texture);
    background.setScale(1.5f, 1.5f);
    if (!texture2.loadFromFile("myasset/rectcut.png")) {
        std::cout << "ERROR TEXTURE" << std::endl;
    }
    rect = sf::Sprite(texture2);
    rect.setPosition(0, -10);
    tag = sf::Sprite(texture2);
    tag.setScale(-1.0f, 0.5f);
    if (!texture3.loadFromFile("myasset/starship.png")) {
        std::cout << "ERROR TEXTURE" << std::endl;
    }
    starship = sf::Sprite(texture3);
    starship.setPosition(positionx, positiony);
    starship.setScale(2.0f, 2.0f);
    if (!texture4.loadFromFile("myasset/rect.png")) {
        std::cout << "ERROR TEXTURE" << std::endl;
    }
    bigrect = sf::Sprite(texture4);
    bigrect.setPosition(-70, 30);
    bigrect.setScale(1.4f, 0.9f);
    if (!font.loadFromFile("myasset/space font.ttf")) {
        std::cout << "ERROR FONT" << std::endl;
    }
    text.setFont(font);
    text.setString("Room name:");
    text.setCharacterSize(30);
    text.setPosition(350, 555);
    front_promt.setFont(font);
    front_promt.setCharacterSize(30);
    front_promt.setPosition(350, 80);
    front_promt.setColor(sf::Color::Black);
    roomname.setFont(font);
    roomname.setCharacterSize(50);
    roomname.setPosition(10, 5);
    exit_room.setFont(font);
    exit_room.setString("Exit");
    exit_room.setCharacterSize(70);
    exit_room.setPosition(720+ 150 , 630);
    play.setFont(font);
    play.setString("Play");
    play.setCharacterSize(70);
    play.setPosition(200 + 150 , 630);
    pointer[0] = 130;
    pointer[1] = 660;
    pointer[2] = 545;
};


int Room::event(sf::Event event , sf::RenderWindow *window) {
    if((event.type == sf::Event::TextEntered || event.text.unicode == 8)) {
        promt = event_to_string(event, promt);
        text.setString( "Me : " + promt);
    }
    if(starship.getPosition().x == pointer[0])
    {
        if (event.key.code == sf::Keyboard::Up)
            starship.setPosition(pointer[0] ,pointer[2]);
        else if (event.key.code == sf::Keyboard::Down)
            starship.setPosition(pointer[0] , positiony);
    } else if(starship.getPosition().y != pointer[2])
    {
        if (event.key.code == sf::Keyboard::Left) {
            index = (index + 1) % 2;
            std::cout << index << std::endl;
            starship.setPosition(pointer[index], positiony);
        }
        else if (event.key.code == sf::Keyboard::Right) {
            index = (index + 1) % 2;
            starship.setPosition(pointer[index], positiony);
        }
    }
    if (event.key.code == sf::Keyboard::Return) {
        if(starship.getPosition().x == pointer[0] && starship.getPosition().y == positiony) {
            this->rType->network->request("SET_READY", [this](Command &response) {
                std::cout << "Response msg: "<< response.toStr() << std::endl;
            });
        } else if (starship.getPosition().y == pointer[2]){
            std::string onemsg = playername + " : " + promt;
            promt = "";
            print("send message:" + onemsg);
            this->rType->network->request("MESSAGE " + onemsg, [this](Command &response) {
                std::cout << "Response msg: "<< response.toStr() << std::endl;
            });

        }
    } else if (starship.getPosition().y < 550) {
        return ROOM;
    }
    if (event.key.code == sf::Keyboard::Escape) {
        this->rType->network->request("LEAVE_ROOM", [this](Command &response) {
            std::cout << "Response msg: "<< response.toStr() << std::endl;
        });
        return LOBBY;
    }
    return ROOM;
}


void Room::draw(sf::RenderWindow *window) {


    deltaTime = clock.restart().asSeconds();
    fps = 1.f / deltaTime;

    sf::Time delay = sf::milliseconds(1000);
    elapsed_time += r.restart();

    // We have enough time to spawn a sprite. ( may be for several ? )
    while( elapsed_time >= delay ){
        print(std::to_string(fps));
        this->rType->network->request("GET_MESSAGES", [this](Command &response) {
            if (response.getCommand() == "200") {
                std::cout << "Response msg: "<< response.toStr() << std::endl;
                std::string line = response.toStr();
                line = line.substr(3, line.size());
                int len = line.length();
                std::vector<std::string> subArray;

                for (int j = 0, k = 0; j < len; j++) {
                    if (line[j] == '|') {
                        std::string ch = line.substr(k, j - k);
                        k = j+1;
                        subArray.push_back(ch);
                    }
                    if (j == len - 1) {
                        std::string ch = line.substr(k, j - k+1);
                        subArray.push_back(ch);
                    }
                }
                this->chat = subArray;
            } else {
                std::cout << "Error: " << response.toStr() << std::endl;
            }
        });
        this->rType->network->request("GET_READY", [this](Command &response) {
            if (response.getCommand() == "200") {
                std::cout << "Response READY: " << response.toStr() << std::endl;
                std::string line = response.toStr();
                line = line.substr(3, line.size());
                int len = line.length();
                std::vector <std::string> subArray;

                for (int j = 0, k = 0; j < len; j++) {
                    if (line[j] == '|') {
                        std::string ch = line.substr(k, j - k);
                        k = j + 1;
                        subArray.push_back(ch);
                    }
                    if (j == len - 1) {
                        std::string ch = line.substr(k, j - k + 1);
                        subArray.push_back(ch);
                    }
                }
                this->player = subArray;
            } else {
                std::cout << "Error: " << response.toStr() << std::endl;
            }
        });

        elapsed_time -= delay;
    }

    roomname.setString(str_roomname);
    this->x -= 0.015;
    this->background.setPosition(this->x,0);
    if(starship.getPosition().x == pointer[0] && starship.getPosition().y != pointer[2]) {
        play.setColor(sf::Color::Red);
        play.setCharacterSize(75);
        exit_room.setColor(sf::Color::White);
        exit_room.setCharacterSize(70);
    } else if (starship.getPosition().x == pointer[1]){
        play.setColor(sf::Color::White);
        play.setCharacterSize(70);
        exit_room.setColor(sf::Color::Red);
        exit_room.setCharacterSize(75);
    } else if (starship.getPosition().y == pointer[2]) {
        play.setColor(sf::Color::White);
        play.setCharacterSize(70);
    }
    window->draw(this->background);
    window->draw(this->play);
    window->draw(this->rect);
    window->draw(this->roomname);
    window->draw(this->bigrect);
    int point = 0;
    front_promt.setColor(sf::Color::Black);
    for (int i = chat.size();  i != 0 && (chat.size() - 10) != i ; i--) {
        if(chat.size() > 10) {
            point = i - ( chat.size()  - 10 );
        }
        else
            point = i;
        front_promt.setString(chat[i - 1]);
        front_promt.setPosition(350, 80 + (39 * point ));
        window->draw(this->front_promt);
    }
    window->draw(this->text);
    window->draw(this->exit_room);
    window->draw(this->starship);
    front_promt.setColor(sf::Color::White);
    for(int i = 0; nb_player != i; i++) {
        tag.setPosition(1290, 100 + (i * 60));
        window->draw(tag);
        front_promt.setString( i < this->player.size() ? this->player.at(i) : "");
        front_promt.setPosition(900, 115 + (i * 60));
        window->draw(this->front_promt);
    }
}