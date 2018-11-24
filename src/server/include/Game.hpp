//
// Created by David Zakrzewski on 17/11/2018.
//

#ifndef R_TYPE_GAME_H
#define R_TYPE_GAME_H

#include <vector>
#include <string>
#include <ctime>
#include "UDPServer.hpp"
#include "Entities.hpp"

class UDPServer;
using boost::asio::ip::udp;

class Game {
public:
    Game();
    Game(boost::asio::io_context &io, const udp::endpoint &endpoint);
    ~Game();

    void Init();
    void Start();
    void Pause();
    void End();

    void AddPlayer(std::string name);
    void ParseMonsterFile();

    std::vector<Player> &GetPlayers();
    std::vector<Monster> &GetMonsters();
    std::vector<Bullet> &GetBullets();

    void CreateBullet(double x, double y, int speed);

    void CheckAllReady();
    void CheckAllMonsters();

private:
    int _cycle;
    std::vector<Monster> _Monsters;
    std::vector<Player> _Players;
    std::vector<Bullet> _Bullets;
    bool _running;
    UDPServer *_udpServer;
    std::thread *_udpThread;
    int _numberOfPlayers;
    bool _gameStarted;
};

#endif //R_TYPE_GAME_H
