#ifndef SERVER_H
#define SERVER_H
#include "player.h"
#include "iostream"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <map>
#include <conio.h>

std::vector<player> makePlayers(int& amount_players) noexcept;
void playerWalking(const bool update ,std::vector<player> &players);
void do_server(std::string &text, bool &sinitializing,std::vector<player> &players);

#endif // SERVER_H
