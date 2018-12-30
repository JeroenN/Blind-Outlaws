#ifndef SERVER_H
#define SERVER_H
#include "player.h"
#include "bullet.h"
#include "initializing.h"
#include "wall.h"
#include "client.h"
#include "bulletcode.h"
#include "iostream"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <map>
//#include <conio.h>

bool fire_able(int time);
void receive_tcp_messages(sf::TcpSocket &socket, sf::TcpListener &listener);
void bulletHit(std::vector<bullet> &bullets, std::vector<player> players, const int celSize);
void set_shooting_dir(int &shooting_dir);
bool player_check_walking(const std::vector<player> &players, sf::Vector2f prevPosition);

void server_receive_ip_port(sf::TcpSocket &TcpSocket, sf::TcpListener &listener, unsigned short &clientPort, std::vector<unsigned short> &vectorClientPorts,
                             std::vector<std::pair<std::string, int>> &vectorPlayerType, std::map<int, sf::IpAddress> &playerPortIP, bool &tcpMessageReceived);
std::vector<player> makePlayers(int& amount_players, const int celSize) noexcept;
void send_which_team_role_taken(const sf::IpAddress clientIP, const std::vector<unsigned short> vectorClientPorts,
                                const bool tcpMessageReceived, int &m_playersTeam1,int &roleTeam1, int &m_playersTeam2, int &roleTeam2);
void send_client_player_position(sf::IpAddress ip, unsigned short port, const std::vector<player> &players);
void playerWalking(std::vector<player> &players, bool &update, int &time, const int celSize);
void do_server(std::vector<player> &players,std::pair<std::string,int> playerType, const int celSize, sf::RenderWindow &window);

#endif // SERVER_H
