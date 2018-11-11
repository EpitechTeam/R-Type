//
// Created by cherkaoui on 11/11/18.
//

#ifndef R_TYPE_ROOM_HPP
#define R_TYPE_ROOM_HPP
#include "client.hpp"

class Room {

public:
    sf::Texture texture;
    sf::Sprite background;
    sf::Texture texture2;
    sf::Sprite rect;
    sf::Texture texture3;
    sf::Texture texture4;
    sf::Sprite starship;
    sf::Sprite bigrect;
    sf::Font font;
    sf::Text text;
    sf::Text exit_room;
    sf::Text roomname;
    sf::Text play;
    sf::Text front_promt;
    std::string str_roomname = "my_room";
    std::string promt = "";
    std::string playername = "guest";
    int nb_player = 1;
    int int_exit_room = 4;
    // Declaring Vector of String type
    std::vector <std::string> chat;
    std::vector <std::string> player;
/*
    // Initialize vector with strings using push_back
    // command
    colour.push_back("Blue");
    colour.push_back("Red");
    colour.push_back("Orange");
    colour.push_back("Yellow");

    // Print Strings stored in Vector
    for (int i=0; i<colour.size(); i++)
    cout << colour[i] << "\n";*/

    float x  = 0;
    float positionx = 30;
    float positiony = 560;
    sf::Sprite tag;

    Room(){
        if (!texture.loadFromFile("./src/client/myasset/background.jpg")) {
            std::cout << "ERROR TEXTURE" << std::endl;
        }
        background = sf::Sprite(texture);
        background.setScale(1.9f, 1.4f);

        if (!texture2.loadFromFile("./src/client/myasset/rectcut.png")) {
            std::cout << "ERROR TEXTURE" << std::endl;
        }
        rect = sf::Sprite(texture2);
        rect.setPosition(0, -10);
        tag = sf::Sprite(texture2);
        tag.setScale(-1.0f, 0.5f);

        if (!texture3.loadFromFile("./src/client/myasset/starship.png")) {
            std::cout << "ERROR TEXTURE" << std::endl;
        }
        starship = sf::Sprite(texture3);
        starship.setPosition(positionx, positiony);
        starship.setScale(0.3f, 0.3f);


        if (!texture4.loadFromFile("./src/client/myasset/rect.png")) {
            std::cout << "ERROR TEXTURE" << std::endl;
        }
        bigrect = sf::Sprite(texture4);
        bigrect.setPosition(-70, 30);
        bigrect.setScale(1.4f, 0.9f);

        //text

        if (!font.loadFromFile("./src/client/myasset/space font.ttf")) {
            std::cout << "ERROR FONT" << std::endl;
        }
        text.setFont(font);
        text.setString("Room name:");
        text.setCharacterSize(30);
        text.setPosition(250, 555);

        front_promt.setFont(font);
        front_promt.setCharacterSize(30);
        front_promt.setPosition(250, 80);
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

    };

    ~Room(){

    };

    void setRoomName(std::string str) {
        this->str_roomname = str;
    }

    void setPlayerName(std::string str) {
        this->playername = str;
    }

    void handleTextEntered(sf::Uint32 key) {
        if (key >= 128 || key ==  27 || key == 13)
            return;
        if (key ==   8) {
            promt = promt.substr(0, promt.size() != 0 ? promt.size() -1 : 0);
            return;
        }
        promt += static_cast<char>(key);
    }

    int event(sf::Event event , sf::RenderWindow *window){

        if((event.type == sf::Event::TextEntered || event.text.unicode == 8)) {
            handleTextEntered(event.text.unicode);
            text.setString( "Me : " + promt);
        }

        if (event.key.code == sf::Keyboard::Left) {
            starship.move(-4.f, 0.f);
        }
        else if (event.key.code == sf::Keyboard::Right) {
            starship.move(4.f, 0.f);
        }
        else if (event.key.code == sf::Keyboard::Down) {
            starship.move(0.f, 4.f);
        }
        else if (event.key.code == sf::Keyboard::Up) {
            starship.move(0.f, -4.f);
        }
        if (event.key.code == sf::Keyboard::Return)
        {
            chat.push_back(playername + " : " + promt);
            promt = "";
        }
        if (starship.getPosition().y < 550) {
            return ROOM;
        }
        return ROOM;
    }


    void drawPlayer(sf::RenderWindow *window) {
    }


    void draw(sf::RenderWindow *window){
        roomname.setString(str_roomname);
        this->x -= 0.015;
        this->background.setPosition(this->x,0 );
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
            //std::cout <<"size: " << chat.size() << " " << " point: " << point - 1 << " i:" <<  i <<  std::endl;
            front_promt.setString(chat[i - 1]);
            front_promt.setPosition(250, 80 + (39 * point ));
            window->draw(this->front_promt);
        }
        window->draw(this->text);
        window->draw(this->exit_room);
        window->draw(this->starship);
        front_promt.setColor(sf::Color::White);
        for(int i = 0; nb_player != i; i++)
        {
            tag.setPosition(1290, 100 + (i * 60));
            window->draw(tag);
            front_promt.setString("player"+ std::to_string(i + 1) + ": " + playername);
            front_promt.setPosition(900, 115 + (i * 60));
            window->draw(this->front_promt);

        }
    }
};

#endif //R_TYPE_ROOM_HPP
