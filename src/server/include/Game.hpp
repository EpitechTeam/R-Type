//
// Created by David Zakrzewski on 17/11/2018.
//

#ifndef R_TYPE_GAME_H
#define R_TYPE_GAME_H

#include <vector>
#include <string>
#include <ctime>
#include "UDPServer.hpp"

class UDPServer;
using boost::asio::ip::udp;

typedef struct {
    int x;
    int y;
} Position;

class Entity {
public:
    Entity();
    virtual ~Entity();

    int GetLife();
    void SetLife(int life);

    Position GetPosition();
    void SetPosition(Position pos);

private:
    int _life;
    Position _position;
};

class Monster : public Entity {
public:
    Monster();
    ~Monster();

    const std::string& GetType();
    void SetType(const std::string &);

    int GetSpawnCycle();
    void SetSpawnCycle(int);

    int GetFireCycle();
    void SetFireCycle(int);
private:
    std::string _type;
    int _spawnCycle;
    int _fireCycle;
};

class Player : public Entity {
public:

private:
};

class Game {
public:
    Game();
    Game(boost::asio::io_context &io, const udp::endpoint &endpoint);
    ~Game();

    void Init();
    void Start();
    void Pause();
    void End();

    void AddPlayer();
    void ParseMonsterFile();

private:
    int _cycle;
    std::vector<Monster> _Monsters;
    std::vector<Player> _Players;
    bool _running;
    UDPServer *_udpServer;
    std::thread *_udpThread;
};

#endif //R_TYPE_GAME_H
