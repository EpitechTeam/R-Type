//
// Created by cherkaoui on 24/11/18.
//

#include "RType.hpp"
#include "RequestManager.hpp"

RType::RType() {
    this->network = new RequestManager(this);
    window = new sf::RenderWindow(sf::VideoMode(1280, 720), "R * TYPE by [ EZTeam feat BABOU'GAMES ] ®");
}

void RType::connect(const std::string host, const std::string port) {

    std::cout << "host: " << host << " port: " << port << std::endl;
    this->resolver = new tcp::resolver(io_context);
    // tcp::resolver resolver(io_context);
    std::cout << "host: " << host << " port: " << port << std::endl;
    this->endpoints = this->resolver->resolve(host, port);
    std::cout << "host: " << host << " port: " << port << std::endl;
    client = new Client(&io_context, endpoints);

    std::cout << "PASSED:" << __LINE__ << std::endl;

    this->t = new std::thread([this]() {
        this->io_context.run();
        this->auth->create.setString("Retry");
        this->view = AUTH;
    });

    this->inputT = new std::thread([this]() {
        if (this->client->isConnected()) {
            char line[Message::max_body_length + 1];
            while (std::cin.getline(line, Message::max_body_length + 1)) {
                Message msg;
                msg.body_length(std::strlen(line));
                std::memcpy(msg.body(), line, msg.body_length());
                msg.encode_header();
                if (this->network) {
                    this->network->request(msg, [](Command &command) {
                        std::cout << "Response: " << command.toStr() << std::endl;
                    });
                }
            }
        }
    });
}

void RType::draw() {
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
        case 5: {
            room->draw(this->window);
            room->playername = auth->playername;
            room->str_roomname = createRoom->str_roomname;
            room->nb_player = createRoom->int_nbplayer;
            break;
        }
        case 6: {
            if(client && this->client->connected) {
                std::cout << "client => " << "connected" << std::endl;
                std::cout << "connection oklm " << std::endl;
                this->network->request("SET_NAME " + auth->playername, [this](Command &response) {
                    print("set name: " + response.toStr());
                    this->view = LOBBY;
                });
                this->view = LOBBY;

            } else
                std::cout << "client => " <<  "not connected" << std::endl;
            auth->draw(this->window);
            break;
        }
        case 61: {
            std::cout << "lets go" << std::endl;
            std::string delimiter = ":";
            this->ip = this->auth->server_ip.substr(0, this->auth->server_ip.find(delimiter));
            this->port = this->auth->server_ip.substr(this->auth->server_ip.find(delimiter) + 1,
                                                      this->auth->server_ip.length());
            this->connect(ip, port);
            this->auth->create.setString("Wait");
            this->view = AUTH;
            break;
        }
        case 62:
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

void RType::event(sf::Event event) {
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
            case 61:
                break;
            case 62:
                break;
            case 7:
                this->view = game->event(event, window);
                break;
            default:
                this->view = menu->event(event, window);
                break;
        }
    } else if (view == 7) {
        this->view = game->event(event, window);
    }
}

