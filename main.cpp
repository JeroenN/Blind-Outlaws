#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <map>
#include <conio.h>
#include "player.h"
#include "server.h"
#include "bullet.h"

void create_window(const std::string windowName,sf::RenderWindow &window)
{
    const sf::Vector2u windowSize{500,500};
    window.setSize(windowSize);
    window.setTitle(windowName);

    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(true);
}
void do_client(std::vector<player> &players, unsigned short &clientPort, bool &update,sf::RenderWindow &window)
{
    int currentAmountOfBullets=1;
    std::vector<bullet> bullets{makeBullets(currentAmountOfBullets)};
    float bulletX=0;

    sf::Event Event;
    const sf::IpAddress serverIP="192.168.10.70";
    const sf::IpAddress clientIP=sf::IpAddress::getLocalAddress();
    const std::string cIP=clientIP.sf::IpAddress::toString();
    clientPort=2001;

    sf::TcpListener listener;
    sf::UdpSocket socket;
    sf::TcpSocket messageTypeSocket;

    //socket.bind(2001);

    listener.listen(clientPort);
    socket.setBlocking(false);
    listener.setBlocking(false);
    messageTypeSocket.setBlocking(false);

    client_send_ip_port(cIP, clientPort, serverIP);

    while(window.isOpen())
    {
        while(window.pollEvent(Event))
        {
           if(Event.type == sf::Event::GainedFocus)
                update=true;
           if(Event.type == sf::Event::LostFocus)
               update = false;
        }
    sf::Vector2f prevPosition = sf::Vector2f(players[0].getPosX(), players[0].getPosY());
    playerWalking(players, update);

    shoot_bullet(currentAmountOfBullets, bullets, bulletX);

    sf::IpAddress recipient = serverIP;
    unsigned short serverPort = 2000;
    if(player_check_walking(players, prevPosition)==true)
    {
        send_position(recipient, serverPort, players);
    }

    //receive_tcp_messages(messageTypeSocket, listener);
    receive_position_packets(socket, players, bullets);

    draw_everything(window, players, bullets);
    }
}

int main()
{
    std::string clientName = "client";
    std::string serverName = "server";
    unsigned short clientPort;
    int currentAmountOfPlayers=2;
    std::vector<player> players{makePlayers(currentAmountOfPlayers)};
    bool  update =true;
    bool initializing=true;
    std::string connectionType ="";
    std::string serverCheck ="server";
    std::string serverCheckShort ="s";

    std::cout <<"Enter (server) for server, Enter (client) for client: \n";
    std::cin >> connectionType;

    sf::RenderWindow window(sf::VideoMode(500, 500), "SFML window");

        if(connectionType==serverCheckShort || connectionType==serverCheck)
        {
          create_window(serverName, window);
          do_server(initializing, players, update,window);
        }
        else
        {
           create_window(clientName, window);
           do_client(players, clientPort, update,window);
        }
}
