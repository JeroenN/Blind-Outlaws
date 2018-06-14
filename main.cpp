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
#include "initializing.h"

int main()
{
    std::string connectionType;
    connectionType=user_select_server_or_client();
    std::pair<std::string,int> playerType = user_select_player_type(connectionType);
    sf::RenderWindow window(sf::VideoMode(500, 500), "SFML window");
    run_server_or_client(connectionType,playerType, window);
}
