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
void draw_everything(sf::RenderWindow &window, std::vector<player> &players, sf::RectangleShape bullet);
void receive_and_set_other_players_position(sf::UdpSocket &socket, std::vector<player> &players);
bool player_check_walking(std::vector<player> &players,sf::Vector2f prevPosition);
void create_and_shoot_bullet(bool &bulletCreated, sf::RectangleShape &bullet);
void server_receive_ip_port(int &oneTime);
void client_send_ip_port(std::string cIP, unsigned short clientPort, sf::IpAddress serverIP);
std::vector<player> makePlayers(int& amount_players) noexcept;
void send_position(sf::IpAddress ip, unsigned short port, std::vector<player> &players);
void playerWalking(std::vector<player> &players, bool &update);
void do_server(bool &initializing,std::vector<player> &players, bool &update, sf::RenderWindow &window);

#endif // SERVER_H
