#ifndef SERVER_H
#define SERVER_H
#include "player.h"
#include "bullet.h"
#include "initializing.h"
#include "client.h"
#include "iostream"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <map>
//#include <conio.h>

bool fire_able(int time);
void receive_bullet_created(sf::Packet bulletPacket,std::vector<bullet> &bullets, std::vector<player> &players);
void set_player_position(std::vector<player> &players, sf::Packet posPacket);
void set_bullet_position(std::vector<bullet> &bullets, sf::Packet posPacket);
void send_position_bullet(const sf::IpAddress ip, const unsigned short port, std::vector<bullet> &bullets);
void receive_tcp_messages(sf::TcpSocket &socket, sf::TcpListener &listener);
void draw_everything(sf::RenderWindow &window, std::vector<player> &players, std::vector<bullet> &serverBullets,
                     std::vector<bullet> &clientBullets, const std::string role, const int celSize);
void receive_position_packets(sf::UdpSocket &socket, std::vector<player> &players, std::vector<bullet> &bullets,
                              std::pair<int, sf::IpAddress> playerPortIp);
void bulletHit(std::vector<bullet> &bullets, std::vector<player> players, const int celSize);
void set_shooting_dir(int &shooting_dir);
bool player_check_walking(const std::vector<player> &players, sf::Vector2f prevPosition);
void shoot_bullet(std::vector<bullet> &bullets,sf::IpAddress &ip, unsigned short &port, std::vector<player> &players,
                  bool &update, int &time, const int shooting_dir);
void server_receive_ip_port(sf::TcpSocket &TcpSocket, sf::TcpListener &listener,
                            unsigned short &clientPort, bool &clientConnecting);
void client_send_ip_port(std::string cIP, unsigned short clientPort, sf::IpAddress serverIP);
std::vector<player> makePlayers(int& amount_players) noexcept;
void send_player_position(sf::IpAddress ip, std::vector<unsigned short> ports, const std::vector<player> &players);

void send_client_player_position(sf::IpAddress ip, unsigned short port, const std::vector<player> &players);
void playerWalking(std::vector<player> &players, bool &update, int &time, const int celSize);
void do_server(std::vector<player> &players,std::pair<std::string,int> playerType, bool &update, sf::RenderWindow &window);

#endif // SERVER_H
