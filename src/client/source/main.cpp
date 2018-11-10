/*
** EPITECH PROJECT, 2018
** R-Type
** File description:
**        main.cpp
*/

#include <SFML/Graphics.hpp>
#include <iostream>
#include "client.hpp"

class IObject
{
public:
    virtual std::string getObjectName() const = 0;
    virtual bool isActive() const = 0;
    virtual void setActive(bool _active) = 0;
};


class AObject : public IObject
{
protected:
    bool _active;
    std::string _name;

public:
    explicit AObject(bool _active = false, const std::string &_name = "");
    virtual ~AObject() = default;

    std::string getObjectName() const override;
    bool isActive() const override;
    void setActive(bool _active) override;
};

/*
 *
class Menu {

public:
    sf::Sprite background;

    Menu() {
        sf::Texture texture;
        if (!texture.loadFromFile("../src/client/myasset/stars.png"))
        {
            std::cout << "ERROR" << std::endl;
        }
        this->background(texture);
    }

    ~Menu() {
    }

    void draw(sf::RenderWindow window){
        this->DrawBackGround(window);
    }

    void DrawBackGround(sf::RenderWindow window) {
        window.draw(this->background);
    }

};
*/


//TODO SET SIZE
//void setSize(sf::)
#include "Menu.hpp"
#include "Connection.hpp"

class RType {
public:
    sf::RenderWindow *window = 0;
    Menu *menu = new Menu();
    Connection *connection = new Connection();
    int view = 1;
    RType(){
        window = new sf::RenderWindow(sf::VideoMode(1280, 720), "R * TYPE by [ EZTeam feat BABOU'GAMES ] ®");
    }
    ~RType(){
    }

    void  draw(){
        switch(view) {
            case 0: window->close();;
                break;
            case 1: menu->draw(this->window);
                break;
            case 2: connection->draw(this->window);
                break;
            default: menu->draw(this->window);
                break;
        }
    }
    void event(sf::Event event){
        if (event.type == sf::Event::KeyPressed)
        {
            std::cout << "event : " << view << std::endl;
            switch(view) {
                case 0:;
                    break;
                case 1: this->view = menu->event(event, window);
                    break;
                case 2:  this->view = connection->event(event, window);
                    break;
                default: menu->draw(this->window);
                    break;
            }
        }
    }

};

int main()
{
    RType *rType = new RType();

    while (rType->window->isOpen())
    {
        sf::Event event;
        while (rType->window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                rType->window->close();
            rType->event(event);
        }
        rType->window->clear();
        rType->draw();
        rType->window->display();
    }
    return 0;
}