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

void do_client(std::vector<player> &players, std::pair<std::string, int> playerType, const unsigned short clientPort, sf::RenderWindow &window)
{
    bool update=true;
    const std::string role =playerType.first;
    int shooting_dir=0;
    int timeWalking=0;
    int timeShooting=0;
    int celSize=30;
    std::vector<bullet> clientBullets{};
    std::vector<bullet> serverBullets{};
    sf::Event Event;
    const sf::IpAddress serverIP="127.0.0.1"; //local server ip, there should be an option to input the server ip.


    sf::UdpSocket socket;
    socket.bind(clientPort);
    socket.setBlocking(false);

    //client_send_ip_port(cIP, clientPort, serverIP);
    //client_send_playerType(playerType, serverIP);

    while(window.isOpen())
    {
        window_events(window, Event, update); //selecting and deselecting the window and if the user presses escape the window closes

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
            send_client_player_position(recipient, serverPort, players);
        }
        send_position_bullet(recipient, serverPort, clientBullets);

        receive_position_packets(socket, players, serverBullets);

        bulletHit(serverBullets, players, celSize);

        draw_everything(window, players, serverBullets, clientBullets, role, celSize);
    }
}

