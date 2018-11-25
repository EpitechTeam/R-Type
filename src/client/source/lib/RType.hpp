//
// Created by cherkaoui on 24/11/18.
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
    CreateRoom *createRoom = new CreateRoom(this);
    JoinRoom *joinRoom = new JoinRoom();
    Room *room = new Room();
    Auth *auth = new Auth();
    Game *game = new Game();
    RequestManager *network;
    std::string ip = "127.0.0.1";
    std::string port = "4242";
    int view = 1;
    boost::asio::io_context io_context;
    Client *client;
    tcp::resolver *resolver;
    std::thread *t;
    std::thread *inputT;
    tcp::resolver::results_type endpoints;
    bool connected = false;

    RType();

    ~RType() {
        if (this->t) {
            this->t->join();
        }
        if (this->inputT) {
            this->inputT->join();
        }
        this->client->close();
        //inputT.join();
        //inputT.join();
        //inputT.join();
    }

    void connect(const std::string host,const std::string port);

    void draw();

    void event(sf::Event event);
};

#endif //R_TYPE_R_TYPE_HPP
