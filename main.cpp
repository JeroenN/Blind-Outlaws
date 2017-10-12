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

void do_client(bool &initializing,std::vector<player> &players, unsigned short &clientPort, bool &update,sf::RenderWindow &window)
{

    sf::IpAddress serverIP="192.168.10.50";
    sf::IpAddress clientIP=sf::IpAddress::getLocalAddress();
    std::string cIP=clientIP.sf::IpAddress::toString();

    clientPort=2001;

    sf::Packet posPacket;
    sf::UdpSocket socket;

    sf::Vector2f changingPosition;

    socket.bind(2001);

    socket.setBlocking(false);
    //client_send_ip_port(cIP, clientPort, serverIP);

    while(window.isOpen())
    {
        sf::Event Event;
        while(window.pollEvent(Event))
        {
           if(Event.type == sf::Event::GainedFocus)
                update=true;
           if(Event.type == sf::Event::LostFocus)
               update = false;
        }
    playerWalking(players, update);

    sf::IpAddress recipient = serverIP;
    unsigned short serverPort = 2000;
    send_position(recipient, serverPort, players);

    sf::IpAddress sender;
    unsigned short port;
    if (socket.receive(posPacket,sender,port) != sf::Socket::Done)
    {
        //std::cout<<"whoops... some data wasn't received";
    }

    if(posPacket>>changingPosition.x>>changingPosition.y)//>>playerNumber)
    {
        players[1].setPlayerPosition(changingPosition.x, changingPosition.y);
    }
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
    int initializingSeverReceiveFunction=0;
    unsigned short clientPort;
    int currentAmountOfPlayers=2;
    //for(int i=0; i<3; ++i)
    std::vector<player> players{makePlayers(currentAmountOfPlayers)};
    bool  update =true;
    bool initializing=true;
    std::string connectionType ="";
    std::string serverCheck ="server";
    std::string serverCheckShort ="s";


    std::string text = "Connected to: ";

    std::cout <<"Enter (server) for server, Enter (client) for client: \n";
    std::cin >> connectionType;

    sf::RenderWindow window(
       sf::VideoMode(500, 500),
       "multiplayer");

    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(true);
    /*while(window.isOpen())
    {
        sf::Event Event;
        while(window.pollEvent(Event))
        {
           if(Event.type == sf::Event::GainedFocus)
                update=true;
           if(Event.type == sf::Event::LostFocus)
               update = false;
        }*/
        if(connectionType==serverCheckShort || connectionType==serverCheck)
        {
          do_server(initializing, players, update,window);
        }
        else
        {
           do_client(initializing, players, clientPort, update,window);
        }
       /* window.clear();
        for(int i=0; i<static_cast<int>(players.size()); ++i)
        {
            players[i].display(window);
        }
        window.display();
    }*/
}
