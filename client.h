#ifndef CLIENT_H
#define CLIENT_H
#include "iostream"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include "player.h"
#include "server.h"
#include "bullet.h"

void do_client(std::vector<player> &players, std::pair<std::string, int> playerType, const unsigned short clientPort, const int celSize, sf::RenderWindow &window);

#endif // CLIENT_H
