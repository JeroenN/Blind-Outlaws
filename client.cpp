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

void draw_everything(
  sf::RenderWindow &window, std::vector<player> &players, std::vector<bullet> &serverBullets, std::vector<bullet> &clientBullets, const std::string role,
  const int celSize, std::vector<bullet> otherClientBullets, const int bulletsInGun)

{
    window.clear();
    for(int i=0; i<static_cast<int>(clientBullets.size()); ++i)
    {
       clientBullets[i].display(window);
    }
    for(int i=0; i<static_cast<int>(otherClientBullets.size()); ++i)
    {
       otherClientBullets[i].display(window);
    }
    for(int i=0; i<static_cast<int>(serverBullets.size()); ++i)
    {
       serverBullets[i].display(window);
    }

    //if(role == "s" || role =="spectator")
    //{
        for(int i=0; i<static_cast<int>(players.size()); ++i)
        {
            players[i].display(window);
        }
    /*}
    if(role == "p" || role =="player")
    {
            players[0].display(window);
    }*/
    //Grid
    sf::Font font = load_font();
    sf::Text text =create_text("bullets: ",font,10,10,20);
    sf::Text textBullet =create_text(std::to_string(bulletsInGun),font,120,10,20);
    window.draw(textBullet);
    window.draw(text);

    for(int i=0; i<26; ++i)
    {
        sf::Vertex horLine[] =
        {
            sf::Vertex(sf::Vector2f(celSize*i, 0)),
            sf::Vertex(sf::Vector2f(celSize*i, window.getSize().y))
        };
        sf::Vertex verLine[] =
        {
            sf::Vertex(sf::Vector2f(0, celSize*i)),
            sf::Vertex(sf::Vector2f(window.getSize().x, celSize*i))
        };
        window.draw(horLine, 2, sf::Lines);
        window.draw(verLine, 2, sf::Lines);
    }
    window.display();
}

//This is also a send function, should 2 or 3 smaller functions, future me is going to deal with that... in the future
void shoot_bullet(std::vector<bullet> &bullets,sf::IpAddress &ip, unsigned short port, std::vector<player> &players,
                  bool &update, int &time, const int shooting_dir, int &bulletsInGun)
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
     if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && update==true && fire_able(time)==true && bulletsInGun>0)
     {
         bulletsInGun--;
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

void receive_bullet_created(sf::Packet bulletPacket,std::vector<bullet> &bullets, std::vector<bullet> &otherClientBullets, std::vector<player> &players)
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
        if(messageType=="createClientBullet")
        {
            otherClientBullets.push_back(bullet(10,10, players[0].getPosX(), players[0].getPosY(), speedX, speedY));
        }
    }
}

void receive_player_position(std::vector<player> &players, sf::Packet posPacket)
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

void receive_bullet_position(std::vector<bullet> &bullets, sf::Packet posPacket, std::vector<bullet> &clientBullets, std::vector<bullet> &otherClientBullets)
{
    std::string bulletText ="bullet";
    sf::Vector2f changingPosition;
    //std::ostringstream messageType;
    std::string messageReceived;
    for(unsigned int i=0; i<bullets.size(); ++i)
    {
        if(posPacket>> messageReceived>>changingPosition.x>>changingPosition.y)
        {
            std::ostringstream messageType;
            messageType<<bulletText<<i;
            if(messageType.str()==messageReceived)
            {
                bullets[i].setBulletPosition(changingPosition.x, changingPosition.y);
            }

        }
    }
    for(unsigned int i=0; i<otherClientBullets.size(); ++i)
    {
        if(posPacket>> messageReceived>>changingPosition.x>>changingPosition.y)
        {
            std::ostringstream messageType;
            messageType<<"bulletClient"<<i;
            if(messageType.str()==messageReceived)
            {
                std::cout<<"X Position "<< messageType.str() << " :"<< changingPosition.x<<"\n";
                otherClientBullets[i].setBulletPosition(changingPosition.x, changingPosition.y);
            }
        }
    }
}

void receive_position_packets(sf::UdpSocket &socket, std::vector<player> &players, std::vector<bullet> &bullets, std::vector<bullet> &clientBullets, std::vector<bullet> &otherClientBullets)
{
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

        receive_player_position(players, posPacket);
        receive_bullet_position(bullets, posPacket, clientBullets, otherClientBullets);
        receive_bullet_created(posPacket, bullets, otherClientBullets, players);
    }
}

void send_player_position(sf::IpAddress ip, unsigned short port, const std::vector<player> &players)
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
void send_position_bullet(const sf::IpAddress ip, const unsigned short port,
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
    std::vector<bullet> otherClientBullets{};
    std::vector<bullet> serverBullets{};
    sf::Event Event;
    const sf::IpAddress serverIP="127.0.0.1"; //local server ip, there should be an option to input the server ip.
    int bulletsInGun =3;
    int reloadCounter=60;

    sf::UdpSocket socket;
    socket.bind(clientPort);
    socket.setBlocking(false);

    while(window.isOpen())
    {
        ++reloadCounter;
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
            shoot_bullet(clientBullets, recipient, serverPort,players, update, timeShooting, shooting_dir, bulletsInGun);
            if(player_check_walking(players, prevPosition)==true)
            {
                send_player_position(recipient, serverPort, players);
            }
            send_position_bullet(recipient, serverPort, clientBullets);
            reloadBullets(bulletsInGun, reloadCounter);
            //bulletHit(serverBullets, players, celSize);
        }

        receive_position_packets(socket, players, serverBullets, clientBullets, otherClientBullets);
        draw_everything(window, players, serverBullets, clientBullets, role, celSize, otherClientBullets, bulletsInGun);
    }
}


