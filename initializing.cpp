#include "initializing.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <map>
#include <sstream>
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
    int team=0;
    if(connectionType=="server" || connectionType=="s")
    {
        std::cout<<"You are placed in team one \n";
    }
    if(connectionType=="client" || connectionType=="c")
    {
        std::cout<<"Enter (1) to join team 1, Enter (2) to be the player: \n";
        std::cin >> team;
        while(team!=1 && team!=2)
        {
            std::cout<<"This is an invalid command. \nEnter (1) to join team 1, Enter (2) to be the player: \n";
            std::cin >> team;
        }
    }
    std::cout<<"Enter (spectate) to be the spectator, Enter (player) to be the player: \n";
    std::cin >> playerRole;
    while(playerRole!="s" && playerRole!="spectate" && playerRole != "p" && playerRole != "player")
    {
        std::cout<<"This is an invalid command. \nEnter (spectate) to be the spectator, Enter (player) to be the player: \n";
        std::cin >> playerRole;
    }
    playerType = std::make_pair(playerRole, team);

    return playerType;
}

//Program runs the client or the server code based on the previous user choice
void run_server_or_client(std::string const connectionType, std::pair<std::string,int> playerType, sf::RenderWindow &window)
{
    unsigned short clientPort;
    int currentAmountOfPlayers=2;
    std::vector<player> players{makePlayers(currentAmountOfPlayers)};
    std::string serverCheck ="server";
    std::string serverCheckShort ="s";
    bool  update =true;
    if(connectionType==serverCheckShort || connectionType==serverCheck)
    {
      std::string serverName = "server";
      create_window(serverName, window);
      do_server(players, playerType, update,window);
    }
    else
    {
       std::string clientName = "client";
       create_window(clientName, window);
       do_client(players, playerType, clientPort, update,window);
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
