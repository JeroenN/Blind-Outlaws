#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <map>
#include <sstream>
#include <string>
#include "player.h"
#include "server.h"
#include "bullet.h"
#include "initializing.h"
#include "wall.h"
void receive_bullet_created_client(sf::Packet bulletPacket,std::vector<bullet> &bullets, std::vector<player> &players)
{
    std::string messageType=" ";
    int speedX=0;
    int speedY=0;
    if(bulletPacket>>messageType)
    {
        if(messageType=="bulletCreated")
        {
            bullets.push_back(bullet(10,10, players[1].getPosX(), players[1].getPosY(), speedX, speedY));
        }
    }
}

void receive_player_position_client(std::vector<player> &players, sf::Packet posPacket)
{
    sf::Vector2f changingPosition;
    std::string messageType;

    if(posPacket>>changingPosition.x>>changingPosition.y>> messageType) //>> objectNumber -> needs to be added
    {
        if(messageType=="player")
        {
            players[1].setPlayerPosition(changingPosition.x, changingPosition.y);
        }
        if(messageType=="otherPlayer")
        {
            players[0].setPlayerPosition(changingPosition.x, changingPosition.y);
        }
    }
}

void receive_bullet_position_client(std::vector<bullet> &bullets, sf::Packet posPacket,std::string bulletText)
{
    sf::Vector2f changingPosition;
    std::ostringstream messageType;
    std::string messageReceived;
    for(unsigned int i=0; i<bullets.size(); ++i)
    {
        if(posPacket>>changingPosition.x>>changingPosition.y>> messageReceived)
        {
            messageType<<bulletText<<i;
            if(messageType.str()==messageReceived)
            {
                bullets[i].setBulletPosition(changingPosition.x, changingPosition.y);
            }
        }
    }
}

void receive_position_packets_client(sf::UdpSocket &socket, std::vector<player> &players, std::vector<bullet> &bullets)
{

    std::string bulletText ="bullet";
    sf::IpAddress sender;
    unsigned short port;
    sf::Packet posPacket;

    for(int i=0; i<4; ++i)
    {
        if (socket.receive(posPacket,sender,port) != sf::Socket::Done)
        {
            //playerPortIp={port, sender};
            //std::cout<<playerPortIp.second;
            //std::cout<<"whoops... some data wasn't received";
        }

        receive_player_position_client(players, posPacket);
        receive_bullet_position_client(bullets, posPacket, bulletText);
        receive_bullet_created_client(posPacket, bullets, players);
    }
}

void send_player_position_client(sf::IpAddress ip, unsigned short port, const std::vector<player> &players)
{
    std::string playerMessage ="player";
    sf::UdpSocket socket;
    sf::Packet posPacket;
    posPacket<<players[0].getPosX() <<players[0].getPosY()<< playerMessage;//<<playerNumber;

    if (socket.send(posPacket, ip, port) != sf::Socket::Done)
    {
        //std::cout<<"whoops... some data wasn't sent";
    }
}

void do_client(std::vector<player> &players, std::pair<std::string, int> playerType, const unsigned short clientPort, const int celSize, sf::RenderWindow &window)
{
    bool update=true;
    const std::string role =playerType.first;
    int shooting_dir=0;
    int timeWalking=0;
    int timeShooting=0;
    std::vector<bullet> clientBullets{};
    std::vector<bullet> serverBullets{};
    sf::Event Event;
    const sf::IpAddress serverIP="127.0.0.1"; //local server ip, there should be an option to input the server ip.


    sf::UdpSocket socket;
    socket.bind(clientPort);
    socket.setBlocking(false);

    while(window.isOpen())
    {
        window_events(window, Event, update); //selecting and deselecting the window and if the user presses escape the window closes

        if(role =="p"|| role == "player")
        {
            timeWalking+=1;
            timeShooting+=1;
            const sf::Vector2f prevPosition = sf::Vector2f(players[0].getPosX(), players[0].getPosY());
            playerWalking(players, update, timeWalking, celSize);

            sf::IpAddress recipient = serverIP;
            unsigned short serverPort = 2000;
            set_shooting_dir(shooting_dir);
            shoot_bullet(clientBullets, recipient, serverPort,players, update, timeShooting, shooting_dir);
            if(player_check_walking(players, prevPosition)==true)
            {
                send_player_position_client(recipient, serverPort, players);
            }
            send_position_bullet(recipient, serverPort, clientBullets);

            bulletHit(serverBullets, players, celSize);
        }

        receive_position_packets_client(socket, players, serverBullets);
        draw_everything(window, players, serverBullets, clientBullets, role, celSize);
    }
}

