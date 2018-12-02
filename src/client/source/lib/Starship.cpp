//
// Created by cherkaoui on 16/11/18.
//

#include "Starship.hpp"
#include "RType.hpp"
#include "Game.hpp"

Starship::Starship(Game *game, std::string id, int color) : game(game) {
    this->id = id;
    if (!texture3.loadFromFile("myasset/starship.png")) {
        std::cout << "ERROR TEXTURE" << std::endl;
    }
    starship = sf::Sprite(texture3);
    if (color == 1)
        starship.setColor(sf::Color::Green);
    else if (color == 2)
        starship.setColor(sf::Color::Red);
    else if (color == 3)
        starship.setColor(sf::Color::Yellow);
    starship.setScale(2.0f, 2.0f);
};

void Starship::draw(sf::RenderWindow *window, float deltatime,  std::vector<Bullet*> *bullet) {
    window->draw(this->starship);

    if (this->id == this->game->rType->auth->playername)
    {
        elapsed_time += r.restart();
        sf::Time delay = sf::milliseconds(delay_msecond);

        if ( elapsed_time >= delay ) {
            if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) ) {
                sf::Vector2f position = starship.getPosition();
                position.x += 5;
                std::string reqstr = "FIRE_BULLET " + std::to_string(position.x) + " " + std::to_string(position.y);
                game->client->request(reqstr , [this](std::string cmd) {
                });
            }
            elapsed_time = sf::microseconds(0);
        }
        if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Left ) && starship.getPosition().x > 0 ) {
            starship.setPosition(starship.getPosition().x -speed * deltatime, starship.getPosition().y);
            this->game->client->request("MOVE_PLAYER " + std::to_string(starship.getPosition().x) + " " + std::to_string(starship.getPosition().y), [this](std::string cmd) {});
        }
        if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Right ) && starship.getPosition().x < 1220) {
            starship.setPosition(starship.getPosition().x + speed * deltatime, starship.getPosition().y);
            this->game->client->request("MOVE_PLAYER " + std::to_string(starship.getPosition().x) + " " + std::to_string(starship.getPosition().y), [this](std::string cmd) {});
        }
        if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Up ) && starship.getPosition().y > 0) {
            starship.setPosition(starship.getPosition().x, starship.getPosition().y -speed * deltatime);
            this->game->client->request("MOVE_PLAYER " + std::to_string(starship.getPosition().x) + " " + std::to_string(starship.getPosition().y), [this](std::string cmd) {});
        }
        if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Down  ) && starship.getPosition().y < 690) {
            starship.setPosition(starship.getPosition().x, starship.getPosition().y +  speed * deltatime);
            this->game->client->request("MOVE_PLAYER " + std::to_string(starship.getPosition().x) + " " + std::to_string(starship.getPosition().y), [this](std::string cmd) {});
        }
    }
}