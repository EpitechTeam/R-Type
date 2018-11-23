//
// Created by kahoul on 20/11/18.
//

#ifndef R_TYPE_R_TYPE_HPP
#define R_TYPE_R_TYPE_HPP

#include <SFML/Graphics.hpp>

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <iostream>


#include "Client.hpp"
#include "Menu.hpp"
#include "Connection.hpp"
#include "CreateRoom.hpp"
#include "JoinRoom.hpp"
#include "Room.hpp"
#include "Auth.hpp"
#include "Game.hpp"

class RequestManager;

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
    RequestManager *network;
    int view = 1;

    RType(Client &client);

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

#endif //R_TYPE_R_TYPE_HPP
