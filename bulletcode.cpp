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
#include <cassert>
#include <QFile>

int reloadBullets(int &bulletsInGun)
{
    const int maxBulletsInGun=3;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
    {
        std::cout<<"reload";
        return maxBulletsInGun;
    }
    return bulletsInGun;
}
