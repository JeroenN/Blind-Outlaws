#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <map>
#include <conio.h>
//#include <Packet.hpp>
#include "player.h"
#include "server.h"
//#include "player.h"

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
    sf::Event Event;
    sf::IpAddress serverIP="192.168.10.74";
    sf::IpAddress clientIP=sf::IpAddress::getLocalAddress();
    std::string cIP=clientIP.sf::IpAddress::toString();
    clientPort=2001;

    sf::Packet posPacket;
    sf::UdpSocket socket;

    sf::Vector2f changingPosition;

    socket.bind(2001);

    socket.setBlocking(false);
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


    sf::IpAddress recipient = serverIP;
    unsigned short serverPort = 2000;
    if(player_check_walking(players, prevPosition)==true)
    {
        send_position(recipient, serverPort, players);
    }

    receive_and_set_other_players_position(socket, players);
    window.clear();
       for(int i=0; i<static_cast<int>(players.size()); ++i)
       {
           players[i].display(window);
       }
       window.display();
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
