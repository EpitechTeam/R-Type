//
// Created by cherkaoui on 16/11/18.
//

#ifndef R_TYPE_GAME_HPP
#define R_TYPE_GAME_HPP

#include "client.hpp"
#include "Starship.hpp"
#include "Bullet.hpp"
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
    Starship* starship = new Starship[4];
    std::vector<Bullet*> bullet;

    float deltaTime = 0.0f;
    sf::Clock clock;
    float x  = 0;

    Game(){
        if (!texture.loadFromFile("./src/client/myasset/background.png")) {
            std::cout << "ERROR TEXTURE" << std::endl;
        }
        background = sf::Sprite(texture);
       background.setScale(1.5f, 1.5f);

        if (!font.loadFromFile("./src/client/myasset/old.ttf")) {
            std::cout << "ERROR FONT" << std::endl;
        }
        text.setFont(font);
        text.setString("Game name:");
        text.setColor(sf::Color::White);
        text.setCharacterSize(30);
        text.setPosition(20, 650);

        front_promt.setFont(font);
        front_promt.setCharacterSize(30);
        front_promt.setPosition(10, 250);
        front_promt.setColor(sf::Color::White);
    };

    ~Game(){

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


    int event(sf::Event event , sf::RenderWindow *window){

        if((event.type == sf::Event::TextEntered || event.text.unicode == 8)) {
            promt = event_to_string(event, promt);
            text.setString( "cmd : " + promt);
        }
        if (event.key.code == sf::Keyboard::Return) {
            chat.push_back("sudo : " + promt);
            promt = "";
        }

        for(int i  = 0; i != 4; i++) {
            starship[i].event(event,&bullet,window);
        }

        if (event.key.code == sf::Keyboard::Escape) {
            return ROOM;
        }
        return MAP;
    }


    void drawPlayer(sf::RenderWindow *window) {
    }


    float lastTime = 0;
    float fps = 0;


    void draw(sf::RenderWindow *window){

        deltaTime = clock.restart().asSeconds();
        float fps = 1.f / deltaTime;
        lastTime = deltaTime;

        this->x -= 0.018;
        this->background.setPosition(this->x,0 );
        window->draw(this->background);
        int point = 0;
        front_promt.setColor(sf::Color::White);
        for (int i = chat.size();  i != 0 && (chat.size() - 10) != i ; i--) {
            if(chat.size() > 10) {
                point = i - ( chat.size()  - 10 );
            }
            else
                point = i;
            front_promt.setString(chat[i - 1]);
            front_promt.setPosition(20, 200 + (39 * point ));
            window->draw(this->front_promt);
        }

        for (int i = 0;  i != bullet.size(); i++) {
            bullet[i]->draw(window, deltaTime);
        }
        for(int i  = 0; i != 4; i++) {
            starship[i].draw(window);
        }
        std::ostringstream ss;
        ss << fps;
        std::string s(ss.str());

        this->text.setString("fps: " + s + " : " +promt);
        window->draw(this->text);
        front_promt.setColor(sf::Color::White);
    }
};


#endif //R_TYPE_GAME_HPP
