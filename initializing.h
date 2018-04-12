#ifndef INITIALIZING_H
#define INITIALIZING_H
#include "iostream"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include "player.h"
#include "server.h"
#include "bullet.h"
#include "client.h"

void create_window(const std::string windowName,sf::RenderWindow &window);

void do_client(std::vector<player> &players, unsigned short &clientPort, bool &update,sf::RenderWindow &window);
std::string user_select_server_or_client();

//Program runs the client or the server code based on the previous user choice
void run_server_or_client(std::string const connectionType, sf::RenderWindow &window);

void window_events(sf::RenderWindow &window, sf::Event Event, bool &update);
#endif // INITIALIZING_H
