/*
** EPITECH PROJECT, 2018
** R-Type
** File description:
**        main.cpp
*/

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Client.hpp"

class IObject {
public:
    virtual std::string getObjectName() const = 0;

    virtual bool isActive() const = 0;

    virtual void setActive(bool _active) = 0;
};


class AObject : public IObject {
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

#include "Menu.hpp"
#include "Connection.hpp"
#include "CreateRoom.hpp"
#include "JoinRoom.hpp"
#include "Room.hpp"
#include "Auth.hpp"
#include "Game.hpp"

class RType {
public:
    sf::RenderWindow *window = 0;
    Menu *menu = new Menu();
    Connection *connection = new Connection();
    CreateRoom *createRoom = new CreateRoom();
    JoinRoom *joinRoom = new JoinRoom();
    Room *room = new Room();
    Auth *auth = new Auth();
    Game *game = new Game();
    Client *_client;

    int view = 1;

    RType(Client &client)
        : _client(&client)
    {
        window = new sf::RenderWindow(sf::VideoMode(1280, 720), "R * TYPE by [ EZTeam feat BABOU'GAMES ] ®");
    }

    ~RType() {
    }

    void draw() {
        switch (view) {
            case 0:
                window->close();
                break;
            case 1:
                menu->draw(this->window);
                break;
            case 2:
                connection->draw(this->window);
                break;
            case 3:
                createRoom->draw(this->window);
                break;
            case 4:
                joinRoom->draw(this->window);
                break;
                {
                    case 5:
                        room->draw(this->window);
                    room->playername = auth->str_roomname;
                    room->str_roomname = createRoom->str_roomname;
                    room->nb_player = createRoom->int_nbplayer;
                }
                break;
            case 6:
                auth->draw(this->window);
                break;
            case 7:
                game->draw(this->window);
                break;
            default:
                menu->draw(this->window);
                break;
        }
    }

    void event(sf::Event event) {
        if ((event.type == sf::Event::KeyPressed || event.type == sf::Event::TextEntered) && view != 7) {
            //std::cout << this->view << std::endl;
            switch (view) {
                case 0:;
                    break;
                case 1:
                    this->view = menu->event(event, window);
                    break;
                case 2:
                    this->view = connection->event(event, window);
                    break;
                case 3:
                    this->view = createRoom->event(event, window);
                    break;
                case 4:
                    this->view = joinRoom->event(event, window);
                    break;
                case 5:
                    this->view = room->event(event, window);
                    break;
                case 6:
                    this->view = auth->event(event, window);
                    break;
                case 7:
                    this->view = game->event(event, window);
                    break;
                default:
                    menu->draw(this->window);
                    break;
            }
        } else if (view == 7) {
            this->view = game->event(event, window);
        }
    }

};

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "Message.hpp"

int main(int argc, char **argv) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: Client <host> <port>\n";
            return 1;
        }

        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(argv[1], argv[2]);
        Client client(io_context, endpoints);

        std::thread t([&io_context]() { io_context.run(); });

        std::thread inputT([&client]() {

            char line[Message::max_body_length + 1];
            while (std::cin.getline(line, Message::max_body_length + 1)) {
                Message msg;
                msg.body_length(std::strlen(line));
                std::memcpy(msg.body(), line, msg.body_length());
                msg.encode_header();
                client.write(msg);
            }
        });

        RType rType(client);

        while (rType.window->isOpen()) {
            sf::Event event;
            while (rType.window->pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    rType.window->close();
                rType.event(event);
            }
            rType.window->clear();
            rType.draw();
            rType.window->display();
        }

        client.close();
        t.join();
        inputT.join();
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}