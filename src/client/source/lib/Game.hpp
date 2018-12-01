//
// Created by cherkaoui on 16/11/18.
//

#ifndef R_TYPE_GAME_HPP
#define R_TYPE_GAME_HPP

class RequestManager;

#include <Command.hpp>
#include "Client.hpp"

class RType;

#include "Starship.hpp"
#include "Bullet.hpp"
#include "Mob.hpp"
#include "UDPClient.hpp"

class Game {

public:
    sf::Texture texture;
    sf::Sprite background;
    sf::Text front_promt;
    sf::Text text;
    sf::Font font;
    std::string str_roomname = "my_room";
    std::string promt = "";
    std::string playername = "guest";
    std::vector <std::string> chat;
    std::vector <std::string> player;
    std::vector<Bullet*> bullet;
    std::vector<Mob*> mob;
    std::vector<Starship*> starship;
    UDPClient *client;
    std::thread *udpclientIoThread;

    float deltaTime = 0.0f;
    sf::Clock clock;
    float x  = 0;
    RType *rType;

    Game(RType *rType);
    ~Game(){
        udpclientIoThread->join();
    };

    bool MobAleadyExist(std::string id);
    int GetMonsterById(std::string id);

    std::vector<std::string> split(std::string phrase, std::string delimiter) {
        std::vector<std::string> list;
        std::string s = std::string(phrase);
        size_t pos = 0;
        std::string token;
        while ((pos = s.find(delimiter)) != std::string::npos) {
            token = s.substr(0, pos);
            list.push_back(token);
            s.erase(0, pos + delimiter.length());
        }
        list.push_back(s);
        return list;
    }

    void updateView(std::string command);

    void setRoomName(std::string str) {
        this->str_roomname = str;
    }

    void setPlayerName(std::string str) {
        this->playername = str;
    }
    void init_udp();

    int backspace = 0;
    std::string event_to_string(sf::Event event, std::string str){
        sf::Uint32 key = event.text.unicode;

        // std::cout << "key "<< key << "c = " << backspace << std::endl;
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


    int event(sf::Event event , sf::RenderWindow *window){

        if((event.type == sf::Event::TextEntered || event.text.unicode == 8)) {
            promt = event_to_string(event, promt);
            text.setString( "cmd : " + promt);
        }
        if (event.key.code == sf::Keyboard::Return) {
            chat.push_back("sudo : " + promt);
            promt = "";
        }

        for (unsigned int i = 0;  i != starship.size(); i++) {
            starship[i]->event(event,&bullet,window);
        }

        if (event.key.code == sf::Keyboard::Escape) {
            return ROOM;
        }
        return MAP;
    }

    std::vector<std::string> slice(const std::vector<std::string> &v, int start=0, int end=-1) {
        int oldlen = v.size();
        int newlen;

        if (end == -1 or end >= oldlen){
            newlen = oldlen-start;
        } else {
            newlen = end-start;
        }

        std::vector<std::string> nv(newlen);

        for (int i=0; i<newlen; i++) {
            nv[i] = v[start+i];
        }
        return nv;
    }


    void drawPlayer(sf::RenderWindow *window) {
    }

    float fps = 0;
    sf::Clock r;
    sf::Time elapsed_time;

    void draw(sf::RenderWindow *window);
};


#endif //R_TYPE_GAME_HPP
