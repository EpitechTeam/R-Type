//
// Created by kahoul on 10/11/18.
//

#include "../include/Room.hpp"

Room::Room(std::string &name, int maxSlots)
        : _name(name), _maxSlots(maxSlots) {
    std::cout << "Room " << name << " with " << maxSlots << " slots max created." << std::endl;
}

void
Room::join(participant_ptr participant) {
    _participants.insert(participant);
    for (auto msg: _recent_msgs)
        participant->deliver(msg);
}

void
Room::leave(participant_ptr participant) {
    _participants.erase(participant);
}

void
Room::deliver(const Message &msg) {
    _recent_msgs.push_back(msg);
    while (_recent_msgs.size() > max_recent_msgs)
        _recent_msgs.pop_front();

    for (auto participant: _participants)
        participant->deliver(msg);
}