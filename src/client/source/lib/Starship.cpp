//
// Created by cherkaoui on 16/11/18.
//

#include "Starship.hpp"
#include "Game.hpp"

Starship::Starship(Game *game) : game(game) {
    if (!texture3.loadFromFile("myasset/starship.png")) {
        std::cout << "ERROR TEXTURE" << std::endl;
    }
    starship = sf::Sprite(texture3);
    starship.setPosition(310, 10);
    starship.setScale(2.0f, 2.0f);
};

void Starship::draw(sf::RenderWindow *window, float deltatime,  std::vector<Bullet*> *bullet) {
    window->draw(this->starship);

    elapsed_time += r.restart();
    sf::Time delay = sf::milliseconds(delay_msecond);

    if( elapsed_time >= delay ){
        if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) ) {
            sf::Vector2f position = starship.getPosition();
            position.x += 5;
            bullet->emplace_back(new Bullet(position, id, 1150));

            game->client->request("FIRE_BULLET 10 20", [this](std::string cmd) {
                this->game->chat.push_back("res_fire_bullet: " + cmd);
            });
            elapsed_time = sf::microseconds(0);
        }
    }

    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Left ) && starship.getPosition().x > 0 ) {
        starship.setPosition(starship.getPosition().x -speed * deltatime, starship.getPosition().y);
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Right ) && starship.getPosition().x < 1220) {
        starship.setPosition(starship.getPosition().x + speed * deltatime, starship.getPosition().y);
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Up ) && starship.getPosition().y > 0) {
        starship.setPosition(starship.getPosition().x, starship.getPosition().y -speed * deltatime);
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Down  ) && starship.getPosition().y < 690) {
        starship.setPosition(starship.getPosition().x, starship.getPosition().y +  speed * deltatime);
    }
}