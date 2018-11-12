//
// Created by kahoul on 11/11/18.
//

#include "../include/Participant.hpp"

#include "../include/Room.hpp"

void
Participant::setName(std::string &name) {
    this->_name = name;
}

std::string
Participant::getName(void) const {
    return this->_name;
}

void
Participant::setRoom(Room *room) {
    this->_currentRoom = room;
}
