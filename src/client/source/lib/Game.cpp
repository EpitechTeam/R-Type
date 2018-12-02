//
// Created by cherkaoui on 16/11/18.
//

#include "Game.hpp"
#include "RequestManager.hpp"
#include "RType.hpp"

Game::Game(RType *rType) : rType(rType) {
    if (!texture.loadFromFile("myasset/background.png")) {
        std::cout << "ERROR TEXTURE" << std::endl;
    }
    background = sf::Sprite(texture);
    background.setScale(1.5f, 1.5f);

    if (!font.loadFromFile("myasset/old.ttf")) {
        std::cout << "ERROR FONT" << std::endl;
    }
    text.setFont(font);
    text.setString("Game name:");
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(30);
    text.setPosition(20, 650);

    front_promt.setFont(font);
    front_promt.setCharacterSize(30);
    front_promt.setPosition(10, 250);
    front_promt.setFillColor(sf::Color::White);
};

void Game::init_udp()
{
    client = new UDPClient(rType->io_context, rType->ip, std::stoi(rType->port), this);

    udpclientIoThread = new std::thread([this]() {
        rType->io_context.run();
        std::cout << "udp disconnected : " << std::endl;
        this->rType->view = AUTH;
    });

    client->request("INIT_PLAYER " + rType->auth->playername, [this](std::string cmd) {
        std::cout << "connection udp: "<< cmd << std::endl;
    });
}

bool Game::MobAleadyExist(std::string id) {

    for (unsigned int i = 0; i < mob.size(); i++) {
        if (mob[i]->_id == id)
            return (true);
    }
    return (false);
}

bool Game::PlayerAleadyExist(std::string id) {

    for (unsigned int i = 0; i < starship.size(); i++) {
        if (starship[i]->id == id)
            return (true);
    }
    return (false);
}

int Game::GetMonsterById(std::string id) {
    for (unsigned int i = 0; i < mob.size(); i++) {
        if (mob[i]->_id == id)
            return (i);
    }
    return (-1);
}

int Game::GetMonsterByIdFromServer(std::string id) {
    for (unsigned int i = 0; i < mob.size(); i++) {
        if (mob[i]->_id.substr(1, mob[i]->_id.length()) == id)
            return (i);
    }
    return (-1);
}

int Game::GetPlayerById(std::string id) {
    for (unsigned int i = 0; i < starship.size(); i++) {
        if (starship[i]->id == id)
            return (i);
    }
    return (-1);
}

void Game::updateView(std::string command) {
    if(!command.empty()){

        //std::cout << "receive: " << command << std::endl;
        std::vector<std::string> cmd = split(command, " ");
        if (cmd.size() > 0)
        {

            if (cmd[0] == "NEW_BULLET") {
                //std::cout << "Command callback: " << command << std::endl;
                //std::cout << "CMD[2] (position Y) : " << cmd[2] << std::endl;
                //std::cout << "cmd[3]: " << "#" << cmd[3] << "#" << std::endl;
                this->bullet.emplace_back(new Bullet(sf::Vector2f(std::stod(cmd[1]), std::stod(cmd[2])), cmd[3], 1150, cmd[3] == "monster" ? -1 : 1));

            } else if (cmd[0] == "GET_POSITIONS") {
                std::cout << command << std::endl;
                std::vector<std::string> tokens;

                for (unsigned int index = 1; index < cmd.size() - 1; index++) {
                    tokens = split(cmd[index], ":");
                    if (tokens[2] == "-1") {
                        if (MobAleadyExist(tokens[4] + tokens[3]) == false) {
                            mob.emplace_back(new Mob(sf::Vector2f(std::stod(tokens[0]), std::stod(tokens[1])),
                                                     tokens[4] + tokens[3], 1280));
                        } else {
                            if(GetMonsterById(tokens[4] + tokens[3]) != -1)
                            mob[GetMonsterById(tokens[4] + tokens[3])]->_rect.setPosition(
                                    sf::Vector2f(std::stod(tokens[0]), std::stod(tokens[1])));
                        }
                    } else {
                        std::cout << "create other " << tokens[2] << std::endl;
                        if (PlayerAleadyExist(tokens[2]) == false) {
                            std::cout << "create player " << tokens[2] << std::endl;
                            starship.push_back(new Starship(this, tokens[2]));
                        } else {
                            if(GetPlayerById(tokens[4] + tokens[3]) != -1)
                            starship[GetPlayerById(tokens[2])]->starship.setPosition(
                                    sf::Vector2f(std::stod(tokens[0]), std::stod(tokens[1])));
                        }
                    }
                }
                std::cout << "Nombre de starship : " << starship.size() << std::endl;
                std::cout << "Nombre de mosntres : " << mob.size() << std::endl;
            } else if (cmd[0] == "DEAD") {
                std::cout << "erase Monster " << cmd[1] << "index: " << GetMonsterByIdFromServer(cmd[1]) << std::endl;
                if(GetMonsterByIdFromServer(cmd[1]) != -1)
                {
                    std::cout << "clean monster: " << cmd[1] << std::endl;
                    mob.erase(mob.begin() + GetMonsterByIdFromServer(cmd[1]));
                }
            }
        }
        this->chat.push_back("l: " + command);
    }
}


void Game::draw(sf::RenderWindow *window) {
    deltaTime = clock.restart().asSeconds();
    fps = 1.f / deltaTime;
    sf::Time delay = sf::milliseconds(50);
    elapsed_time += r.restart();
    while( elapsed_time >= delay ){
        client->request("GET_POSITIONS", [this](std::string cmd) {
            this->chat.push_back("res_gp : " + cmd);
        });
        elapsed_time -= delay;
    }

    this->x -= 0.018;
    this->background.setPosition(this->x,0 );
    window->draw(this->background);
    int point = 0;
    front_promt.setFillColor(sf::Color(sf::Color::White));


    if(chat.size() > 30){
        chat = this->slice(chat,chat.size() - 20, chat.size());
    }
    for (unsigned int i = chat.size();  i != 0 && (chat.size() - 10) != i ; i--) {
        if(chat.size() > 10) {
            point = i - ( chat.size()  - 10 );
        }
        else
            point = i;
        front_promt.setString(chat[i - 1]);
        front_promt.setPosition(20, 200 + (39 * point ));
        window->draw(this->front_promt);
    }

    //std::cout << "nb mob ===========> " << mob.size() << std::endl;
    for (unsigned int i = 0;  i != mob.size(); i++) {
        if(mob[i] && !mob[i]->draw(window, deltaTime)){
            mob.erase(mob.begin() + i);
            i--;
        } else{
            for (unsigned int j = 0;  j != starship.size(); j++) {
                if(mob[i] && starship[j]->starship.getPosition().x -1 <= mob[i]->_rect.getPosition().x  &&
                   starship[j]->starship.getPosition().x +1 >= mob[i]->_rect.getPosition().x)
                {
                    if(mob[i] && starship[j]->starship.getPosition().y -3 <= mob[i]->_rect.getPosition().y  &&
                       starship[j]->starship.getPosition().y +30 >= mob[i]->_rect.getPosition().y) {
                        std::cout << "collision starship: " << j << std::endl;
                        mob[i]->_rect.setPosition(-100,0);
                    }
                }
            }
        }
    }

    for (unsigned int i = 0;  i != bullet.size(); i++) {
        if(!bullet[i]->draw(window, deltaTime)){
            bullet.erase(bullet.begin() + i);
            i--;
        } else{
            for (unsigned int j = 0;  j != mob.size(); j++) {
                if(mob[j] && mob[j]->_rect.getPosition().x -1 <= bullet[i]->_rect.getPosition().x  &&
                   mob[j]->_rect.getPosition().x +1 >= bullet[i]->_rect.getPosition().x)
                {
                    if(mob[j] && mob[j]->_rect.getPosition().y -3 <= bullet[i]->_rect.getPosition().y  &&
                       mob[j]->_rect.getPosition().y +50 >= bullet[i]->_rect.getPosition().y) {
                        std::cout << "cout::request " <<  "DEAD " + mob[j]->_id.substr(1, mob[j]->_id.length()) << std::endl;
                        client->request("DEAD " + mob[j]->_id.substr(1, mob[j]->_id.length()), [this](std::string cmd) {
                            //std::cout << "DEAD udp: "<< cmd << std::endl;
                        });
                        bullet[i]->_rect.setPosition(1280,0);
                    }
                }
            }
        }

    }

    for (unsigned int i = 0;  i != starship.size(); i++) {
        starship[i]->draw(window, deltaTime, &bullet);
    }

    std::ostringstream ss;
    ss << fps;
    std::string s(ss.str());
    this->text.setString("fps: " + s + " : " + promt);
    window->draw(this->text);
    front_promt.setFillColor(sf::Color(sf::Color::White));
}