/*
** EPITECH PROJECT, 2018
** R-Type
** File description:
**        client.hpp
*/

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#define  MENU 1
#define  LOBBY 2
#define  CREATEROOM 3
#define  JOINROOM 4
#define  ROOM 5
#define AUTH 6
#define  MAP 7

sf::Sprite createSprite(const std::string path);
void print(std::string string);
int roll(int min, int max);