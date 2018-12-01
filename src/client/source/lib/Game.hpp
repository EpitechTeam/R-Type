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

    void updateView(std::string command) {
        if(!command.empty()){
            std::cout << "non empyreceive: " << command << std::endl;
            std::vector<std::string> cmd = split(command, " ");
            if(cmd.size() > 0)
            {
                if(cmd[0] == "NEW_BULLET") {
                    this->bullet.emplace_back(new Bullet(sf::Vector2f(std::stod(cmd[1]),std::stod(cmd[2])), cmd[3], 1150, cmd[3] == "monster" ? -1 : 1 ));

                }
            }
            this->chat.push_back("l: " + command);
        }
    }

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

    void draw(sf::RenderWindow *window) {
        deltaTime = clock.restart().asSeconds();
        fps = 1.f / deltaTime;
        sf::Time delay = sf::milliseconds(200);
        elapsed_time += r.restart();
        while( elapsed_time >= delay ){
            //sf::Vector2f position = starship[0]->starship.getPosition();
            // position.x = 1280;
            //mob.emplace_back(new Mob(position, std::to_string((rand() % 3) + 1) + std::to_string(mob.size()), 1280));
            // mob.emplace_back(new Mob(position, std::to_string(3) + std::to_string(mob.size()), 1280));
            // Substract the time consumed


            /*client->request("GET_POSITIONS", [this](std::string cmd) {
                  this->chat.push_back("res_gp : " + cmd);
              });*/
            elapsed_time -= delay;
        }

        this->x -= 0.018;
        this->background.setPosition(this->x,0 );
        window->draw(this->background);
        int point = 0;
        front_promt.setFillColor(sf::Color(sf::Color::White));


        if(chat.size() > 30){
            chat = this->slice(chat,chat.size() - 20, chat.size());
        }
        for (unsigned int i = chat.size();  i != 0 && (chat.size() - 10) != i ; i--) {
            if(chat.size() > 10) {
                point = i - ( chat.size()  - 10 );
            }
            else
                point = i;
            front_promt.setString(chat[i - 1]);
            front_promt.setPosition(20, 200 + (39 * point ));
            window->draw(this->front_promt);
        }

        for (unsigned int i = 0;  i != mob.size(); i++) {
            if(!mob[i]->draw(window, deltaTime)){
                mob.erase(mob.begin() + i);
                i--;
            } else{
                for (unsigned int j = 0;  j != starship.size(); j++) {
                    if(starship[j]->starship.getPosition().x -1 <= mob[i]->_rect.getPosition().x  &&
                       starship[j]->starship.getPosition().x +1 >= mob[i]->_rect.getPosition().x)
                    {
                        if(starship[j]->starship.getPosition().y -3 <= mob[i]->_rect.getPosition().y  &&
                           starship[j]->starship.getPosition().y +30 >= mob[i]->_rect.getPosition().y) {
                            std::cout << "collision starship: " << j << std::endl;
                            mob[i]->_rect.setPosition(-100,0);
                        }
                    }
                }
            }
        }

        for (unsigned int i = 0;  i != bullet.size(); i++) {
            if(!bullet[i]->draw(window, deltaTime)){
                bullet.erase(bullet.begin() + i);
                i--;
            } else{
                for (unsigned int j = 0;  j != mob.size(); j++) {
                    if(mob[j]->_rect.getPosition().x -1 <= bullet[i]->_rect.getPosition().x  &&
                       mob[j]->_rect.getPosition().x +1 >= bullet[i]->_rect.getPosition().x)
                    {
                        if(mob[j]->_rect.getPosition().y -3 <= bullet[i]->_rect.getPosition().y  &&
                           mob[j]->_rect.getPosition().y +50 >= bullet[i]->_rect.getPosition().y) {
                            mob.erase(mob.begin() + j);
                            j--;
                            bullet[i]->_rect.setPosition(1280,0);
                        }
                    }
                }
            }

        }

        for (unsigned int i = 0;  i != starship.size(); i++) {
            starship[i]->draw(window, deltaTime, &bullet);
        }

        std::ostringstream ss;
        ss << fps;
        std::string s(ss.str());
        this->text.setString("fps: " + s + " : " + promt);
        window->draw(this->text);
        front_promt.setFillColor(sf::Color(sf::Color::White));
    }
};


#endif //R_TYPE_GAME_HPP
