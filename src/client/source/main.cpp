/*
** EPITECH PROJECT, 2018
** R-Type
** File description:
**        main.cpp
*/

#include "RType.hpp"
#include "RequestManager.hpp"
#include "Client.hpp"

RType::RType(Client &client)
    :_client(&client)
{
    this->network = new RequestManager(*this);
    window = new sf::RenderWindow(sf::VideoMode(1280, 720), "R * TYPE by [ EZTeam feat BABOU'GAMES ] ®");
}

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

        RType rType(client);

        std::thread t([&io_context]() { io_context.run(); });

       /* std::thread inputT([&rType]() {
            char line[Message::max_body_length + 1];
            while (std::cin.getline(line, Message::max_body_length + 1)) {
                Message msg;
                msg.body_length(std::strlen(line));
                std::memcpy(msg.body(), line, msg.body_length());
                msg.encode_header();

                rType.network->request(msg, [](Command &response) {

                    std::cout << "Response: " << response.toStr() << std::endl;
                });
                // client.write(msg);
            }
        });*/

        while (rType.window->isOpen()) {
            sf::Event event;
            while (rType.window->pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    rType.window->close();
                rType.event(event);
            }
            /*rType.network->request("GET_ROOMS", [](Command &response) {

                std::cout << "Response: "<< response.toStr() << std::endl;
            });*/
            rType.window->clear();
            rType.draw();
            rType.window->display();
        }

        //inputT.join();
        client.close();
        t.join();
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}