//
// Created by David Zakrzewski on 17/11/2018.
//

#include <UDPGame.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "UDPServer.hpp"

UDPGame::UDPGame(boost::asio::io_context &io, const udp::endpoint &endpoint)
    : _cycle(0), _running(true), _numberOfPlayers(0), _gameStarted(false) {

    _udpThread = new std::thread([this, &io, &endpoint]() {

        boost::asio::io_context io_context;

        _udpServer = new UDPServer(io_context, endpoint, this);

        io_context.run();
    });

    this->Init();
}

UDPGame::~UDPGame() {
    _udpThread->join();
    delete(_udpServer);
    delete(_udpThread);
}

/*
 * Cette fonction initialize les informations du jeu
 * --> Position des monstres au spawn
 */
void UDPGame::Init() {
    this->ParseMonsterFile();
}

/*
 * Main UDPGame loop
 */
void UDPGame::Start() {
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
            CheckAllMonsters();
            _cycle++;
            clock1 = clock2;
        }
    }
}

void UDPGame::Pause() {
    _running = false;
}

void UDPGame::End() {
}

/*
 * Added automatically by UDPServer
 */
void UDPGame::AddPlayer(std::string id) {
    Player player;
    Position pos;
    static int i = 0;

    _numberOfPlayers++;
    player.SetLife(MAX_LIFE_POINTS);
    switch (_numberOfPlayers) {
        case 1:
            pos = {SCREEN_HEIGHT / 2, PLAYER_START_Y};
        break;
        case 2:
            pos = {SCREEN_HEIGHT / 3 + _Players[0].GetPosition().y, PLAYER_START_Y};
        break;
        case 3:
            pos = {SCREEN_HEIGHT / 4 + _Players[1].GetPosition().y, PLAYER_START_Y};
        break;
        case 4:
            pos = {SCREEN_HEIGHT / 5 + _Players[2].GetPosition().y, PLAYER_START_Y};
        break;
    }
    player.SetPosition(pos);
    player.SetId(id);
    player.SetScore(0);
    player.SetAsset(std::to_string(i));
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

void UDPGame::ParseMonsterFile() {
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

std::vector<Player> &UDPGame::GetPlayers() {
    return _Players;
}

std::vector<Monster> &UDPGame::GetMonsters() {
    return _Monsters;
}

std::vector<Bullet> &UDPGame::GetBullets() {
    return _Bullets;
}

void UDPGame::CreateBullet(double x, double y, int speed) {
    static int i = 0;
    Bullet bullet;

    bullet.SetId(std::to_string(i));
    bullet.SetPosition({x, y});
    bullet.SetSpeed(speed);
    _Bullets.push_back(bullet);
    std::cout << "Bullet " << i << " created." << std::endl;
    i++;
}

void UDPGame::CheckAllReady() {
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

void UDPGame::CheckAllMonsters() {
    Bullet bullet;

    for (auto &monster : _Monsters) {
        if (monster.GetWaitingCycle() <= monster.GetFireCycle()) {
            if (monster.GetWaitingCycle() == monster.GetFireCycle()) {
                bullet.SetPosition({monster.GetPosition().x, monster.GetPosition().y});
                bullet.SetSpeed(monster.GetSpeedFromType(monster.GetType()));
                _udpServer->NewBullet(bullet.GetPosition().x, bullet.GetPosition().y, bullet.GetSpeed());
            }
            monster.SetWaitingCycle(monster.GetWaitingCycle() + 1);
        }
        else {
            monster.SetWaitingCycle(0);
        }
    }
}