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

void reloadBullets(int &bulletsInGun, int &reloadCounter)
{
    const int maxBulletsInGun=3;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
    {
        reloadCounter=0;
    }
    if(reloadCounter==60)
    {
       bulletsInGun =maxBulletsInGun;
    }
}

int bulletSpeedX(const int shooting_dir)
{
    switch(shooting_dir)
    {
        case 0:
        return 3;
        break;
        case 1:
        return -3;
        break;
        case 2:
        return 0;
        break;
        case 3:
        return 0;
        break;
    }
}
int bulletSpeedY(const int shooting_dir)
{
    switch(shooting_dir)
    {
        case 0:
        return 0;
        break;
        case 1:
        return 0;
        break;
        case 2:
        return -3;
        break;
        case 3:
        return 3;
        break;
    }
}
