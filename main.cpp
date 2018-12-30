#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <map>
#include <sstream>
#include <cassert>
#include "player.h"
#include "server.h"
#include "bullet.h"
#include "initializing.h"

int main()
{
    std::string connectionType=user_select_server_or_client();
    run_server_or_client(connectionType);
}

