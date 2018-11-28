//
// Created by cherkaoui on 11/11/18.
//

#ifndef R_TYPE_ROOM_HPP
#define R_TYPE_ROOM_HPPERROR: Failed requirement 'sfml/2.5.1@bincrafters/stable' from 'PROJECT'
ERROR: Unable to find 'sfml/2.5.1@bincrafters/stable' in remotes


class RequestManager;

#include <Command.hpp>
#include "Client.hpp"
#include "UDPClient.hpp"


class RType;

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
    cout << colour[i] << "\n";
*/
    float x  = 0;
    float positionx = 130;
    float positiony = 645;
    sf::Sprite tag;
    float deltaTime = 0.0f;
    sf::Clock clock;
    sf::Clock r;
    sf::Time elapsed_time;
    float fps = 0;

    int *pointer = new int[2];
    RType *rType;
    Room(RType *rType);
    ~Room(){
    };

    void setRoomName(std::string str) {
        this->str_roomname = str;
    }

    void setPlayerName(std::string str) {
        this->playername = str;
    }

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

    int index  = 0;
    int event(sf::Event event , sf::RenderWindow *window);
    void draw(sf::RenderWindow *window);
};

#endif //R_TYPE_ROOM_HPP
