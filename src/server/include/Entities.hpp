//
// Created by David Zakrzewski on 23/11/2018.
//

#ifndef R_TYPE_ENTITIES_H
#define R_TYPE_ENTITIES_H

#include <string>
#include <vector>

#define MAX_LIFE_POINTS 3

typedef struct {
    double x;
    double y;
} Position;

class Entity {
public:
    Entity();
    virtual ~Entity();

    int GetLife();
    void SetLife(int life);

    Position GetPosition();
    void SetPosition(Position pos);

    std::string GetId();
    void SetId(std::string id);

private:
    int _life;
    Position _position;
    std::string _id;
};

class Monster : public Entity {
public:
    Monster();
    ~Monster();

    const std::string& GetType();
    void SetType(const std::string &);

    static int GetSpeedFromType(const std::string& type);

    int GetSpawnCycle();
    void SetSpawnCycle(int);

    int GetFireCycle();
    void SetFireCycle(int);

    int GetWaitingCycle();
    void SetWaitingCycle(int waitingCycle);

    int GetSpeed();
    void SetSpeed(int);

    int GetStyle();
    void SetStyle(int style);
private:
    std::string _type;
    int _style;
    int _spawnCycle;
    int _fireCycle;
    int _speed;
    int _waitingCycle;
};

class Player : public Entity {
public:
    Player();
    ~Player();

    int GetScore();
    void SetScore(int score);

    std::string GetAsset();
    void SetAsset(std::string asset);

    bool IsReady();
    void SetReady(bool ready);

private:
    int _score;
    std::string _asset;
    bool _isReady;
};

class Bullet : public Entity {
public:
    Bullet();
    ~Bullet();

    int GetSpeed();
    void SetSpeed(int speed);

private:
    int _speed;
};

#endif //R_TYPE_ENTITIES_H
