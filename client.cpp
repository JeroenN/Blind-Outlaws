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
//This is also a send function, should 2 or 3 smaller functions, future me is going to deal with that... in the future
void shoot_bullet_client(std::vector<bullet> &bullets,sf::IpAddress &ip, unsigned short port, std::vector<player> &players,
                  bool &update, int &time, const int shooting_dir)
{
    int bulletSpeedX=3;
    int bulletSpeedY=0;
    switch(shooting_dir)
    {
        case 0:
        bulletSpeedX=3;
        bulletSpeedY=0;
        break;
        case 1:
        bulletSpeedX=-3;
        bulletSpeedY=0;
        break;
        case 2:
        bulletSpeedX=0;
        bulletSpeedY=-3;
        break;
        case 3:
        bulletSpeedX=0;
        bulletSpeedY=3;
        break;
    }
     sf::Packet posPacket;
     if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && update==true && fire_able(time)==true)
     {
         bullets.push_back(bullet(10,10, players[0].getPosX()+10, players[0].getPosY()+10, bulletSpeedX, bulletSpeedY));
         std::string bulletMessage ="bulletCreated";
         sf::UdpSocket socket;
         posPacket<<bulletMessage;

         if (socket.send(posPacket, ip, port) != sf::Socket::Done)
         {
             //std::cout<<"whoops... some data wasn't sent";
         }

         time=0;
     }
     if(bullets.size()>0)
     {
        for(unsigned int i=0; i<bullets.size(); ++i)
        {
            bullets[i].moveBullet();
        }
     }
}

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

    if(posPacket>> messageType>>changingPosition.x>>changingPosition.y) //>> objectNumber -> needs to be added
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

void receive_bullet_position_client(std::vector<bullet> &bullets, sf::Packet posPacket,std::string bulletText, std::vector<bullet> &clientBullets)
{
    sf::Vector2f changingPosition;
    std::ostringstream messageType;
    std::string messageReceived;
    for(unsigned int i=0; i<bullets.size(); ++i)
    {
        if(posPacket>> messageReceived>>changingPosition.x>>changingPosition.y)
        {
            messageType<<bulletText<<i;
            if(messageType.str()==messageReceived)
            {
                bullets[i].setBulletPosition(changingPosition.x, changingPosition.y);
            }

        }
    }
    for(unsigned int i=0; i<clientBullets.size(); ++i)
    {
        if(posPacket>> messageReceived>>changingPosition.x>>changingPosition.y)
        {
            messageType<<"bulletClient"<<i;
            if(messageType.str()==messageReceived)
            {
                clientBullets[i].setBulletPosition(changingPosition.x, changingPosition.y);
            }
        }
    }
}

void receive_position_packets_client(sf::UdpSocket &socket, std::vector<player> &players, std::vector<bullet> &bullets, std::vector<bullet> &clientBullets)
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
        receive_bullet_position_client(bullets, posPacket, bulletText, clientBullets);
        receive_bullet_created_client(posPacket, bullets, players);
    }
}

void send_player_position_client(sf::IpAddress ip, unsigned short port, const std::vector<player> &players)
{
    std::string playerMessage ="player";
    sf::UdpSocket socket;
    sf::Packet posPacket;
    posPacket<<playerMessage<<players[0].getPosX() <<players[0].getPosY();//<<playerNumber;

    if (socket.send(posPacket, ip, port) != sf::Socket::Done)
    {
        //std::cout<<"whoops... some data wasn't sent";
    }
}
void send_position_bullet_client(const sf::IpAddress ip, const unsigned short port,
                          std::vector<bullet> &bullets)
{
    std::ostringstream bulletMessage;
    std::string bulletText="bullet";

    sf::UdpSocket socket;
    sf::Packet posPacket;
    if(bullets.size()>0)
    {
        for (unsigned i=0; i<bullets.size(); i++)
        {
            bulletMessage<<bulletText<<i;
            posPacket<< bulletMessage.str()<<bullets[i].getPosX() << bullets[i].getPosY();
        }

        if (socket.send(posPacket, ip, port) != sf::Socket::Done)
        {
            //std::cout<<"whoops... some data wasn't sent";
        }
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
            shoot_bullet_client(clientBullets, recipient, serverPort,players, update, timeShooting, shooting_dir);
            if(player_check_walking(players, prevPosition)==true)
            {
                send_player_position_client(recipient, serverPort, players);
            }
            send_position_bullet_client(recipient, serverPort, clientBullets);

            bulletHit(serverBullets, players, celSize);
        }

        receive_position_packets_client(socket, players, serverBullets, clientBullets);
        draw_everything(window, players, serverBullets, clientBullets, role, celSize);
    }
}

