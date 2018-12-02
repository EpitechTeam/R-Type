//
// Created by kahoul on 10/11/18.
//

#include "../include/Room.hpp"

Room::Room(const std::string &name, const int maxSlots)
        : _name(name), _maxSlots(maxSlots), _game(name) {
    std::cout << "Room " << name << " with " << maxSlots << " slots max created." << std::endl;
}

void
Room::join(participant_ptr participant) {
    _participants.insert(participant);
    participant->setRoom(this);
    std::cout << participant->getName() << " enter the room " << this->getName() << "." << std::endl;
    /*for (auto msg: _recent_msgs)
        participant->deliver(msg);*/
}

void
Room::leave(participant_ptr participant) {
    std::cout << participant->getName() << " leave the room " << this->getName() << "." << std::endl;
    participant->setRoom(NULL);
    _participants.erase(participant);
}

void
Room::deliver(const Message &msg) {
    _recent_msgs.push_back(msg);
   while (_recent_msgs.size() > max_recent_msgs)
        _recent_msgs.pop_front();
/*
    for (auto participant: _participants)
        participant->deliver(msg);*/
}

std::string
Room::getMessages() {
    std::string tmp;

    for (auto it : this->_recent_msgs) {
        tmp.append(it.body(), it.body_length());
        tmp.push_back('|');
    }
    return tmp;
}

bool Room::isAllPlayerReady() const {

    if (this->_participants.size() < (unsigned)_maxSlots) {
        return false;
    }

    for (auto it : this->_participants) {
        if (!it->getReady()) {
            return false;
        }
    }
    return true;
}
