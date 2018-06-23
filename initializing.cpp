#include "initializing.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <map>
#include <sstream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>
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

std::string user_select_server_or_client()
{
    std::string connectionType;
    std::cout <<"Enter (server) for server, Enter (client) for client: \n";
    std::cin >> connectionType;
    while(connectionType!="s" && connectionType!="server" && connectionType != "c" && connectionType != "client")
    {
        std::cout<<"This is an invalid command. \nEnter (server) for server, Enter (client) for client: \n";
        std::cin >> connectionType;
    }
    return connectionType;
}

std::pair <std::string,int> user_select_player_type(std::string connectionType)
{
    std::pair <std::string,int> playerType;
    std::string playerRole="NULL";
    int team=1;
    if(connectionType=="server" || connectionType=="s")
    {
        std::cout<<"You are placed in team one \n";
    }
    if(connectionType=="client" || connectionType=="c")
    {
        std::cout<<"Enter (1) to join team 1, Enter (2) to join team 2: \n";
        std::cin >> team;     //REMOVE COMMENT
        //team=1;
        while(team!=1 && team!=2)
        {
            std::cout<<"This is an invalid command. \nEnter (1) to join team 1, Enter (2) to joing team 2: \n";
            std::cin >> team;
        }
    }
    std::cout<<"Enter (spectate) to be the spectator, Enter (player) to be the player: \n";
    std::cin >> playerRole; //REMOVE COMMENT
    //playerRole="p";
    while(playerRole!="s" && playerRole!="spectate" && playerRole != "p" && playerRole != "player")
    {
        std::cout<<"This is an invalid command. \nEnter (spectate) to be the spectator, Enter (player) to be the player: \n";
        std::cin >> playerRole;
    }
    playerType = std::make_pair(playerRole, team);

    return playerType;
}
void client_send_ip_port(std::string cIP, unsigned short clientPort, sf::IpAddress serverIP)
{
    std::string messageType="IpPort";
    sf::TcpSocket TcpSocket;
    sf::Packet cIPandPortPacket;

    TcpSocket.connect(serverIP,2000);
    cIPandPortPacket<<messageType<<cIP<<clientPort;
    TcpSocket.send(cIPandPortPacket);
}
void client_send_playerType(const std::pair<std::string, int> playerType, const sf::IpAddress serverIP)
{
    std::string messageType="Port";
    sf::TcpSocket TcpSocket;
    sf::Packet playerTypePacket;

    TcpSocket.connect(serverIP,2000);
    playerTypePacket<<messageType<<playerType.first<<playerType.second;
    TcpSocket.send(playerTypePacket);
}
void receive_playerTypes_taken(sf::TcpSocket &TcpSocket, sf::TcpListener &listener, int team1Taken, int role1Taken, int team2Taken, int role2Taken)
{
    std::string messageType;
    sf::Packet tcpPacket;
    listener.accept(TcpSocket);

    if(TcpSocket.receive(tcpPacket)==sf::Socket::Done)
    {
        if(tcpPacket>>messageType)
        {
            if(messageType=="type_team_1")
            {
                std::cout<<"Connected! \n";
                if(tcpPacket>>team1Taken>>role1Taken)
                {
                    if(team1Taken==2)
                    {
                        std::cout<<"TEAM 1 is full, you are automatically placed in TEAM 2";
                    }
                    switch(role1Taken)
                    {
                        case 1:
                        std::cout<<"You can join TEAM 1 and select PLAYER" << "\n \n"<<std::flush;
                        break;
                        case 2:
                        std::cout<<"You can join TEAM 1 and select SPECTATOR" << "\n \n"<<std::flush;
                        break;
                        case 3:
                        std::cout<< "\n \n"<<std::flush;
                        break;
                        default:
                        std::cout<< "You can join TEAM 1 and select PLAYER OR SPECTATOR" << "\n \n"<<std::flush;
                        break;
                    }
                }
            }
            if(tcpPacket>>team2Taken>>role2Taken)
            {
                if(team2Taken==2)
                {
                    std::cout<<"TEAM 2 is full, you are automatically placed in TEAM 1";
                }
                switch(role2Taken)
                {
                    case 1:
                    std::cout<<"You can join TEAM 2 and select PLAYER" << "\n \n"<<std::flush;
                    break;
                    case 2:
                    std::cout<<"You can join TEAM 2 and select SPECTATOR" << "\n \n"<<std::flush;
                    break;
                    case 3:
                    std::cout<< "\n \n"<<std::flush;
                    break;
                    default:
                    std::cout<< "You can join TEAM 2 and select PLAYER OR SPECTATOR" << "\n \n"<<std::flush;
                    break;
                }
            }
        }
    }
}

void choose_playerType(std::pair<std::string,int> &playerType, const unsigned short clientPort, std::string const connectionType)
{
    const sf::IpAddress clientIP=sf::IpAddress::getLocalAddress();
    const std::string cIP=clientIP.sf::IpAddress::toString();
    const sf::IpAddress serverIP="127.0.0.1";

    sf::TcpListener listener;
    sf::TcpSocket TcpSocket;
    listener.listen(clientPort);
    //listener.setBlocking(false);
    //TcpSocket.setBlocking(false);
    int team1Taken;
    int role1Taken;
    int team2Taken;
    int role2Taken;

    client_send_ip_port(cIP, clientPort, serverIP);
    std::cout<<"\nWaiting for response from server... \n";
    receive_playerTypes_taken(TcpSocket, listener, team1Taken, role1Taken, team2Taken, role2Taken);
    playerType = user_select_player_type(connectionType);
    client_send_playerType(playerType, serverIP);

}
//Program runs the client or the server code based on the previous user choice
void run_server_or_client(std::string const connectionType)
{
    std::pair<std::string,int> playerType;
    int currentAmountOfPlayers=2;
    std::vector<player> players{makePlayers(currentAmountOfPlayers)};
    std::string serverCheck ="server";
    std::string serverCheckShort ="s";

    /* initialize random seed: */
    srand (time(NULL));
    unsigned short clientPort = rand() % 99999 + 1000;

    if(connectionType==serverCheckShort || connectionType==serverCheck)
    {
      std::string serverName = "server";
       playerType = user_select_player_type(connectionType);
       sf::RenderWindow window(sf::VideoMode(500, 500), "SFML window");
      create_window(serverName, window);
      do_server(players, playerType, window);
    }
    else
    {
       std::string clientName = "client";
       choose_playerType(playerType, clientPort, connectionType);
       sf::RenderWindow window(sf::VideoMode(500, 500), "SFML window");
       create_window(clientName, window);
       do_client(players, playerType, clientPort,window);
    }
}

//Gain focus of the window clicked on, makes sure that when you run the program on the same computer you don´t affect both windows with the keyboard
void window_events(sf::RenderWindow &window, sf::Event Event, bool &update)
{
    while(window.pollEvent(Event))
    {
           if(Event.type == sf::Event::GainedFocus)
                update=true;
           if(Event.type == sf::Event::LostFocus)
               update = false;
           if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
               window.close();
    }
}
