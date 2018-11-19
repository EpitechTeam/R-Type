//4
// Created by cherkaoui on 10/11/18.
//

#ifndef R_TYPE_JOINROOM_HPP
#define R_TYPE_JOINROOM_HPP

#include "Client.hpp"

class JoinRoom {

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
    float x  = 0;
    float positionx = 30;
    float positiony = 442;

    JoinRoom(){
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
        text.setFont(font); // font est un sf::Font
        text.setString("server:");
        text.setCharacterSize(120);
        text.setPosition(250, 190 + 200);
        text2.setFont(font);
        text2.setString("room name:");
        text2.setCharacterSize(120);
        text2.setPosition(250, 320 + 200);
    };

    ~JoinRoom(){

    };


    int event(sf::Event event , sf::RenderWindow *window){
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
            if (starship.getPosition().y > 550) {
                return 3;
            } else {
                return 2;
            }
        }
        return 4;
    }

    void draw(sf::RenderWindow *window){
        this->x -= 0.015;
        this->background.setPosition(this->x,0 );

        if (starship.getPosition().y > 550) {
            text2.setColor(sf::Color::Red);
            text2.setCharacterSize(128);
            text.setColor(sf::Color::White);
            text.setCharacterSize(120);
        } else {
            text.setColor(sf::Color::Red);
            text.setCharacterSize(128);
            text2.setColor(sf::Color::White);
            text2.setCharacterSize(120);
        }

        window->draw(this->background);
        window->draw(this->text);
        window->draw(this->text2);
        window->draw(this->logo);
        window->draw(this->starship);
    }
};
#endif //R_TYPE_JOINROOM_HPP
