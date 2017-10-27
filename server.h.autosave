#ifndef SERVER_H
#define SERVER_H
#include "player.h"
#include "bullet.h"
#include "iostream"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <map>
#include <conio.h>

void set_player_position(std::vector<player> &players, sf::Packet posPacket);
void set_bullet_position(sf::RectangleShape &bullet, sf::Packet posPacket);
void send_position_bullet(sf::IpAddress ip, unsigned short port, sf::RectangleShape &bullet);
void receive_tcp_messages(sf::TcpSocket &socket, sf::TcpListener &listener);
void draw_everything(sf::RenderWindow &window, std::vector<player> &players, sf::RectangleShape bullet);
void receive_position_packets(sf::UdpSocket &socket, std::vector<player> &players, sf::RectangleShape &bullet);
bool player_check_walking(std::vector<player> &players,sf::Vector2f prevPosition);
void create_and_shoot_bullet(bool &bulletCreated, sf::RectangleShape &bullet, float &bulletX);
void server_receive_ip_port(sf::TcpSocket &TcpSocket, sf::TcpListener &listener,unsigned short &clientPort);
void client_send_ip_port(std::string cIP, unsigned short clientPort, sf::IpAddress serverIP);
std::vector<player> makePlayers(int& amount_players) noexcept;
std::vector<bullet> makeBullets(int& amount_bullets) noexcept;
void send_position(sf::IpAddress ip, unsigned short port, std::vector<player> &players);
void playerWalking(std::vector<player> &players, bool &update);
void do_server(bool &initializing,std::vector<player> &players, bool &update, sf::RenderWindow &window);

#endif // SERVER_H
