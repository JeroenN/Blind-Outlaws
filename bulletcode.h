#ifndef BULLETCODE_H
#define BULLETCODE_H
#include "iostream"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include "player.h"
#include "wall.h"
#include "server.h"
#include "bullet.h"
#include "client.h"

void reloadBullets(int &bulletsInGun,int &reloadCounter);
int bulletSpeedX(const int shooting_dir);
int bulletSpeedY(const int shooting_dir);
void moveAllBullets(std::vector<bullet> &bullets);
#endif // BULLETCODE_H

