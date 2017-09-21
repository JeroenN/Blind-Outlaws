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


void do_client(sf::TcpListener &listenerForClients, bool &initializing,std::vector<player> &players)
{
        float posXplayer1;
        float posYplayer1;
        if(initializing==true)
        {
             posXplayer1=100;
             posYplayer1=110;
             initializing=false;
        }
        int currentAmountOfPlayers=2;
        sf::TcpSocket TcpSocket;
        sf::IpAddress clientIP=sf::IpAddress::getLocalAddress();
        std::string cIP=clientIP.sf::IpAddress::toString();
        sf::IpAddress severIP=sf::IpAddress::getLocalAddress();
        sf::UdpSocket socket;

        sf::Packet playerAssignNumber;
        sf::Uint16 playerNumberClient;

        sf::Packet cIPandPortPacket;

        sf::TcpSocket tcpSocketClient1;
        sf::TcpSocket tcpSocketClient2;


       // unsigned short port;
        unsigned short clientPort;

        std::string sIP="192.168.10.163"; //IP hardcoded, needs to change
        //std::cout << "Enter server ip: "<< "\n";
        //std::cin>> sIP;
        severIP=sIP;
        if(initializing==true)
        {
            std::cout << "set port number: ";
            std::cin >> clientPort;
            initializing=false;
        }
        socket.bind(clientPort);
        //listenerForClient1.listen(clientPort);
        listenerForClients.listen(clientPort);
        TcpSocket.connect(severIP,2000);
        cIPandPortPacket<<cIP<<clientPort;
        TcpSocket.send(cIPandPortPacket);
        //}
        sf::Vector2f prevPosition;
        sf::Vector2f changingPosition;
        TcpSocket.setBlocking(false);
        tcpSocketClient1.setBlocking(false);
        tcpSocketClient2.setBlocking(false);
        socket.setBlocking(false);
        bool update = true;

        prevPosition = sf::Vector2f(players[0].getPosX(), players[0].getPosY());

        playerWalking(update, players);

        sf::Packet posPacket;

            listenerForClients.accept(tcpSocketClient1);
            if(tcpSocketClient1.receive(playerAssignNumber)==sf::Socket::Done)
            {
                std::cout<<"tcpSocketClient1 received a message!  \n";
                if(playerAssignNumber>>playerNumberClient)
                {
                    std::cout<<playerNumberClient<< "\n";
                }
            }



            if(prevPosition != sf::Vector2f(players[0].getPosX(), players[0].getPosY()))
            {
                sf::IpAddress recipient = severIP;
                unsigned short serverPort = 2000;
                posPacket<<players[0].getPosX() <<players[0].getPosY();//<<playerNumber;
                if (socket.send(posPacket, recipient, serverPort) != sf::Socket::Done)
                {
                    //std::cout<<"whoops... some data wasn't sent";
                }

            }

        sf::IpAddress sender;
        unsigned short port;
        if (socket.receive(posPacket,sender,port) != sf::Socket::Done)
        {
            //std::cout<<"whoops... some data wasn't received";
        }
        if(posPacket>>changingPosition.x>>changingPosition.y)//>>playerNumber)
        {
            std::cout<<"staph";
            players[1].setPlayerPosition(changingPosition.x, changingPosition.y);
        }
    }
/*listenerForClients.accept(tcpSocketClient2);
if(tcpSocketClient2.receive(playerAssignNumber)==sf::Socket::Done)
{
    std::cout<<"tcpSocketClient2 received a message! \n";
    if(playerAssignNumber>>playerNumberClient)
    {
        std::cout<<playerNumberClient<< "\n";
    }
}*/


int main()
{

    int currentAmountOfPlayers=2;
    //for(int i=0; i<3; ++i)
    std::vector<player> players{makePlayers(currentAmountOfPlayers)};
    bool  update =true;
    bool initializing=true;
    std::string connectionType ="";
    std::string serverCheck ="server";
    std::string serverCheckShort ="s";

    sf::TcpListener listenerForClients;
    listenerForClients.setBlocking(false);

    sf::TcpListener listenerForClient1;
    listenerForClient1.setBlocking(false);

    std::string text = "Connected to: ";

    std::cout <<"Enter (server) for server, Enter (client) for client: \n";
    std::cin >> connectionType;

    sf::RenderWindow window(
       sf::VideoMode(500, 500),
       "multiplayer");

    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(true);
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
        if(connectionType==serverCheckShort || connectionType==serverCheck)
        {
          do_server(text, initializing, players);
        }
        else
        {
           do_client(listenerForClients, initializing, players);
        }
        window.clear();
        for(int i=0; i<static_cast<int>(players.size()); ++i)
        {
            players[i].display(window);
        }
        window.display();
    }
}
