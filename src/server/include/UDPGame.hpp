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

#define SCREEN_HEIGHT 720
#define PLAYER_START_Y 20

class UDPServer;
using boost::asio::ip::udp;

class UDPGame {
public:
    UDPGame(const std::string &roomName);
    ~UDPGame();

    void Init();
    void Start();
    void Pause();
    void End();

    void AddPlayer(std::string name);
    void ParseMonsterFile();

    std::vector<Player> &GetPlayers();
    std::vector<Monster> &GetMonsters();
    std::vector<Bullet> &GetBullets();

    void CreateBullet(double x, double y, std::string owner);

    void CheckAllReady();
    void CheckAllMonsters();

    void KillMonster(std::string id);

    bool isGameStarted() const;

    void MoveMonsters();

    int getPort() const {
        return this->_port;
    }

private:
    int _port;
    int _cycle;
    std::vector<Monster> _Monsters;
    std::vector<Player> _Players;
    std::vector<Bullet> _Bullets;
    bool _running;
    UDPServer *_udpServer;
    std::thread *_udpThread;
    std::thread *_gameThread;
    int _numberOfPlayers;
    bool _gameStarted;
};

#endif //R_TYPE_GAME_H
