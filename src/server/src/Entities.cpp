//
// Created by David Zakrzewski on 23/11/2018.
//

#include "Entities.hpp"

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

std::string Entity::GetId() {
    return (_id);
}

void Entity::SetId(std::string id) {
    _id = id;
}

Monster::Monster()
        : _waitingCycle(0), _spawned(false) {

};

Monster::~Monster() = default;

const std::string& Monster::GetType() {
    return (_type);
}
void Monster::SetType(const std::string& type) {
    _type = type;
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

int Monster::GetSpeed() {
    return (_speed);
}

void Monster::SetSpeed(int speed) {
    _speed = speed;
}

int Monster::GetSpeedFromType(const std::string& type) {
    if (type == "normal") {
        return (8);
    }
    if (type == "runner") {
        return (20);
    }
    if (type == "robot") {
        return (10);
    }
    return (0);
}

int Monster::GetWaitingCycle() {
    return (_waitingCycle);
}

void Monster::SetWaitingCycle(int waiting) {
    _waitingCycle = waiting;
}

int Monster::GetStyle() {
    return (_style);
}

void Monster::SetStyle(int style) {
    _style = style;
}

void Monster::Spawn() {
    _spawned = true;
}

bool Monster::isSpawned() {
    return (_spawned);
}

std::string Player::GetAsset() {
    return (_asset);
}

Player::Player()
        : _score(0), _asset(""), _isReady(false){
}

Player::~Player() = default;

void Player::SetAsset(std::string asset) {
    _asset = asset;
}

bool Player::IsReady() {
    return (_isReady);
}

void Player::SetReady(bool ready) {
    _isReady = ready;
}

int Player::GetScore() {
    return (_score);
}

void Player::SetScore(int score) {
    _score = score;
}

Bullet::Bullet() = default;

Bullet::~Bullet() = default;

int Bullet::GetSpeed() {
    return (_speed);
}

void Bullet::SetSpeed(int speed) {
    _speed = speed;
}

std::string Bullet::GetOwner() {
    return (this->_owner);
}

void Bullet::SetOwner(std::string owner) {
    _owner = owner;
}