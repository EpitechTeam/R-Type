//
// Created by David Zakrzewski on 17/11/2018.
//

#include <Game.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

Entity::Entity() = default;

Entity::~Entity() = default;


int Entity::GetLife() {
    return (_life);
}

void Entity::SetLife(int life) {
    _life = life;
}

Position Entity::GetPosition() {
    return (_position);
}

void Entity::SetPosition(Position pos) {
    _position = pos;
}

const std::string& Monster::GetType() {
    return (_type);
}
void Monster::SetType(const std::string& type) {
    _type = type;
}

Monster::Monster() {

}

Monster::~Monster() {

}

int Monster::GetSpawnCycle() {
    return (_spawnCycle);
}

void Monster::SetSpawnCycle(int cycle) {
    _spawnCycle = cycle;
}

int Monster::GetFireCycle() {
    return (_fireCycle);
}

void Monster::SetFireCycle(int cycle) {
    _fireCycle = cycle;
}

Game::Game(boost::asio::io_context &io, const udp::endpoint &endpoint)
    : _cycle(0), _running(true) {

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
    for (auto monster : _Monsters) {
        std::cout << monster.GetType() << " " << monster.GetSpawnCycle() << " ";
        std::cout << monster.GetFireCycle() << " " << monster.GetLife() << " ";
        std::cout << monster.GetPosition().x << "," << monster.GetPosition().y << std::endl;
    }
}

void Game::Start() {
    std::clock_t clock1;
    std::clock_t clock2;
    std::clock_t ticks;
    double delta;

    clock1 = clock();
    while (_running) {
        clock2 = clock();
        ticks = clock2 - clock1;
        delta = ticks / (double) (CLOCKS_PER_SEC);
        if (delta >= 0.5) {
            _udpServer->Test();
            std::cout << _cycle << std::endl;
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

void Game::AddPlayer() {
    Player player;

    player.SetLife(100);
    player.SetPosition({10,20});
    _Players.push_back(player);
}

std::vector<std::string> split(std::string phrase, std::string delimiter){
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
            is >> val; // SpawnCycle
            monster.SetSpawnCycle(val);
            is >> tmp_string; // Position
            std::vector<std::string> positions = split(tmp_string, ",");
            pos.x = std::stoi(positions[0]);
            pos.y = std::stoi(positions[1]);
            monster.SetPosition(pos);
            is >> val; // FireCycle
            monster.SetFireCycle(val);
            monster.SetLife(100);
            _Monsters.push_back(monster);
        }
    }
}