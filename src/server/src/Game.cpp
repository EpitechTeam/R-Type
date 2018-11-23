//
// Created by David Zakrzewski on 17/11/2018.
//

#include <Game.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "UDPServer.hpp"

Game::Game(boost::asio::io_context &io, const udp::endpoint &endpoint)
    : _cycle(0), _running(true), _numberOfPlayers(0), _gameStarted(false) {

    _udpThread = new std::thread([this, &io, &endpoint]() {

        boost::asio::io_context io_context;

        _udpServer = new UDPServer(io_context, endpoint, this);

        io_context.run();
    });

    this->Init();
}

Game::~Game() {
    _udpThread->join();
}

/*
 * Cette fonction initialize les informations du jeu
 * --> Position des monstres au spawn
 */
void Game::Init() {
    this->ParseMonsterFile();
}

/*
 * Main Game loop
 */
void Game::Start() {
    std::clock_t clock1;
    std::clock_t clock2;
    std::clock_t ticks;
    double delta;

    clock1 = clock();
    while (_running) {
        CheckAllReady();
        clock2 = clock();
        ticks = clock2 - clock1;
        delta = ticks / (double) (CLOCKS_PER_SEC);
        if (delta >= 0.5) {
            _cycle++;
            clock1 = clock2;
        }
    }
}

void Game::Pause() {
    _running = false;
}

void Game::End() {
}

/*
 * Added automatically by UDPServer
 */
void Game::AddPlayer(std::string id) {
    Player player;
    Position pos;
    double y = 20;
    static int i = 0;

    _numberOfPlayers++;
    player.SetLife(MAX_LIFE_POINTS);
    switch (_numberOfPlayers) {
        case 1:
            pos = {720 / 2, 20};
        break;
        case 2:
            pos = {720 / 3 + _Players[0].GetPosition().y, y};
        break;
        case 3:
            pos = {720 / 4 + _Players[1].GetPosition().y, y};
        break;
        case 4:
            pos = {720 / 5 + _Players[2].GetPosition().y, 20};
        break;
    }
    player.SetPosition(pos);
    player.SetId(id);
    player.SetScore(0);
    player.SetAsset("spaceship" + std::to_string(i) + ".png");
    i++;
    _Players.push_back(player);
}

std::vector<std::string> split(std::string phrase, std::string delimiter) {
    std::vector<std::string> list;
    std::string s = std::string(phrase);
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        list.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    list.push_back(s);
    return list;
}

void Game::ParseMonsterFile() {
    std::ifstream File;
    std::string line;
    Monster monster;
    std::string tmp_string;
    int val;
    Position pos;
    int i = 0;

    File.open("monsters");
    if (!File) {
        std::cout << "Unable to open file";
        exit(1);
    }
    while (getline(File, line)) {
        if (line.at(0) != '#') {
            std::istringstream is(line);
            is >> tmp_string; // Type
            monster.SetType(tmp_string);
            if (monster.GetType() == "normal") {
                is >> val; // SpawnCycle
                monster.SetSpawnCycle(val);
                is >> tmp_string; // Position
                std::vector<std::string> positions = split(tmp_string, ",");
                pos.x = std::stoi(positions[0]);
                pos.y = std::stoi(positions[1]);
                monster.SetPosition(pos);
                is >> val; // FireCycle
                monster.SetFireCycle(val);
                monster.SetSpeed(Monster::GetSpeedFromType(monster.GetType()));
            }
            monster.SetLife(100);
            monster.SetId(std::to_string(i));
            _Monsters.push_back(monster);
            i++;
        }
    }
}

std::vector<Player> &Game::GetPlayers() {
    return _Players;
}

std::vector<Monster> &Game::GetMonsters() {
    return _Monsters;
}

std::vector<Bullet> &Game::GetBullets() {
    return _Bullets;
}

Game::Game() = default;

void Game::CreateBullet(double x, double y, int speed) {
    static int i = 0;
    Bullet bullet;

    bullet.SetId(std::to_string(i));
    bullet.SetPosition({x, y});
    bullet.SetSpeed(speed);
    _Bullets.push_back(bullet);
    std::cout << "Bullet " << i << " created." << std::endl;
    i++;
}

void Game::CheckAllReady() {
    int readyNumber = 0;

    if (_Players.size() > 0 && !_gameStarted) {
        for (auto player : _Players) {
            if (player.IsReady())
                readyNumber++;
        }
        if (readyNumber == _Players.size()) {
            _udpServer->SendToAll("GAME_START\n");
            _gameStarted = true;
        }
    }
}