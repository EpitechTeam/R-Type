//
// Created by cherkaoui on 10/11/18.
//

#ifndef R_TYPE_MENU_HPP
#define R_TYPE_MENU_HPP

#include "Client.hpp"

class Menu {

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
    int *pointer = new int[2];

    Menu(){
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

        //text

        if (!font.loadFromFile("myasset/space font.ttf")) {
            std::cout << "ERROR FONT" << std::endl;
        }
        text.setFont(font); // font est un sf::Font
        text.setString("multyplayer");
        text.setCharacterSize(120);
        text.setPosition(140, 180 + 200);
        text2.setFont(font);
        text2.setString("Exit");
        text2.setCharacterSize(120);
        text2.setPosition(300, 350 + 200);


        pointer[0] = 442;
        pointer[1] = 600;
    };

    ~Menu(){

    };

    int index = 0;
    int event(sf::Event event , sf::RenderWindow *window){
        if (event.key.code == sf::Keyboard::Up) {
            index = (index + 1) % 2;
            std::cout << index << std::endl;
            starship.setPosition(positionx, pointer[index]);
        }
        else if (event.key.code == sf::Keyboard::Down) {
            index = (index + 1) % 2;
            starship.setPosition(positionx, pointer[index]);
        }
        if (event.key.code == sf::Keyboard::Return) {
            if (starship.getPosition().y > 550) {
                return 0;
            } else {
                return AUTH;
            }
        }
        if (event.key.code == sf::Keyboard::Escape) {
            return 0;
        }
        return MENU;
    }

    void draw(sf::RenderWindow *window){
        this->x -= 0.015;
        this->background.setPosition(this->x,0 );

        if (starship.getPosition().y > 550) {
            text2.setFillColor(sf::Color::Red);
            text2.setCharacterSize(128);
            text.setFillColor(sf::Color::White);
            text.setCharacterSize(120);
        } else {
            text.setFillColor(sf::Color::Red);
            text.setCharacterSize(128);
            text2.setFillColor(sf::Color::White);
            text2.setCharacterSize(120);
        }

        window->draw(this->background);
        window->draw(this->text);
        window->draw(this->text2);
        window->draw(this->logo);
        window->draw(this->starship);
    }
};

#endif //R_TYPE_MENU_HPP