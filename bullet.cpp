#include "bullet.h"

bullet::bullet(const float height, const float width, const float posX, const float posY, const float speedX, const float speedY)
    :block(), mPosX(posX), mPosY(posY), mSpeedX(speedX), mSpeedY(speedY)
{
    block.setSize(sf::Vector2f(height, width));
    block.setPosition(sf::Vector2f(posX, posY));
    block.setFillColor(sf::Color(255, 127, 63));
}

